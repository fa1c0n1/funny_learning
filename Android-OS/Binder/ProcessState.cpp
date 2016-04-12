/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "ProcessState"

#include <cutils/process_name.h>

#include <binder/ProcessState.h>

#include <utils/Atomic.h>
#include <binder/BpBinder.h>
#include <binder/IPCThreadState.h>
#include <utils/Log.h>
#include <utils/String8.h>
#include <binder/IServiceManager.h>
#include <utils/String8.h>
#include <utils/threads.h>

#include <private/binder/binder_module.h>
#include <private/binder/Static.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BINDER_VM_SIZE ((1*1024*1024) - (4096 *2))


// ---------------------------------------------------------------------------

namespace android {
 
// Global variables
int                 mArgC;
const char* const*  mArgV;
int                 mArgLen;

class PoolThread : public Thread
{
public:
    PoolThread(bool isMain)
        : mIsMain(isMain)
    {
    }
    
protected:
    virtual bool threadLoop()
    {
        IPCThreadState::self()->joinThreadPool(mIsMain);
        return false;
    }
    
    const bool mIsMain;
};

// 1.单例的ProcessState
// 采用单例，就说明每个进程只有一个ProcessState对象.
sp<ProcessState> ProcessState::self()
{
    Mutex::Autolock _l(gProcessMutex);
    if (gProcess != NULL) {
        return gProcess;
    }
    // 创建一个ProcessState对象，并赋值给gProcess
    gProcess = new ProcessState;
    return gProcess;
}

void ProcessState::setContextObject(const sp<IBinder>& object)
{
    setContextObject(object, String16("default"));
}

sp<IBinder> ProcessState::getContextObject(const sp<IBinder>& caller)
{
    return getStrongProxyForHandle(0);
}

void ProcessState::setContextObject(const sp<IBinder>& object, const String16& name)
{
    AutoMutex _l(mLock);
    mContexts.add(name, object);
}

sp<IBinder> ProcessState::getContextObject(const String16& name, const sp<IBinder>& caller)
{
    mLock.lock();
    sp<IBinder> object(
        mContexts.indexOfKey(name) >= 0 ? mContexts.valueFor(name) : NULL);
    mLock.unlock();
    
    //printf("Getting context object %s for %p\n", String8(name).string(), caller.get());
    
    if (object != NULL) return object;

    // Don't attempt to retrieve contexts if we manage them
    if (mManagesContexts) {
        ALOGE("getContextObject(%s) failed, but we manage the contexts!\n",
            String8(name).string());
        return NULL;
    }
    
    IPCThreadState* ipc = IPCThreadState::self();
    {
        Parcel data, reply;
        // no interface token on this magic transaction
        data.writeString16(name);
        data.writeStrongBinder(caller);
        status_t result = ipc->transact(0 /*magic*/, 0, data, &reply, 0);
        if (result == NO_ERROR) {
            object = reply.readStrongBinder();
        }
    }
    
    ipc->flushCommands();
    
    if (object != NULL) setContextObject(object, name);
    return object;
}

void ProcessState::startThreadPool()
{
    AutoMutex _l(mLock);
    if (!mThreadPoolStarted) {
        mThreadPoolStarted = true;
        spawnPooledThread(true);
    }
}

bool ProcessState::isContextManager(void) const
{
    return mManagesContexts;
}

bool ProcessState::becomeContextManager(context_check_func checkFunc, void* userData)
{
    if (!mManagesContexts) {
        AutoMutex _l(mLock);
        mBinderContextCheckFunc = checkFunc;
        mBinderContextUserData = userData;

        int dummy = 0;
        status_t result = ioctl(mDriverFD, BINDER_SET_CONTEXT_MGR, &dummy);
        if (result == 0) {
            mManagesContexts = true;
        } else if (result == -1) {
            mBinderContextCheckFunc = NULL;
            mBinderContextUserData = NULL;
            ALOGE("Binder ioctl to become context manager failed: %s\n", strerror(errno));
        }
    }
    return mManagesContexts;
}

ProcessState::handle_entry* ProcessState::lookupHandleLocked(int32_t handle)
{
    const size_t N=mHandleToObject.size();
    if (N <= (size_t)handle) {
        handle_entry e;
        e.binder = NULL;
        e.refs = NULL;
        status_t err = mHandleToObject.insertAt(e, N, handle+1-N);
        if (err < NO_ERROR) return NULL;
    }
    return &mHandleToObject.editItemAt(handle);
}

/*
 * 参数handle,类似于Windows编程中经常使用的对资源的一种标识.
 * 说白了,其实就是一个资源项. 
 * 该资源项, 保存在一个资源数组(也可以是别的组织结构)中, handle的值正是该资源项在数组中的索引
 */
sp<IBinder> ProcessState::getStrongProxyForHandle(int32_t handle)
{
    sp<IBinder> result;

    AutoMutex _l(mLock);

    /*
    * 根据索引查找对应资源.
    *   如果lookupHandleLocked发现没有对应的资源项，则会创建一个新的项并返回.
    *   这个新项的内容需要填充
    */
    handle_entry* e = lookupHandleLocked(handle);

    if (e != NULL) {
        // We need to create a new BpBinder if there isn't currently one, OR we
        // are unable to acquire a weak reference on this current one.  See comment
        // in getWeakProxyForHandle() for more info about this.
        IBinder* b = e->binder;
        if (b == NULL || !e->refs->attemptIncWeak(this)) {
            //对于新创建的资源项, 它的binder为空(NULL),所以走这个分支
            b = new BpBinder(handle); //创建一个BpBinder
            e->binder = b; //填充entry的内容
            if (b) e->refs = b->getWeakRefs();
            result = b;
        } else {
            // This little bit of nastyness is to allow us to add a primary
            // reference to the remote proxy when this team doesn't have one
            // but another team is sending the handle to us.
            result.force_set(b);
            e->refs->decWeak(this);
        }
    }

    /*
     * BpBinder是客户端与Server交互的代理类,p即Proxy的意思
     * BBinder是BpBinder交互的目的端
     * BpBinder与BBinder是一一对应的.
     * 
     * BpBinder如何标识它所对应的BBinder ?
     * 答: Binder系统通过handle来对应BBinder
     *
     * 这里handle传进来的值是0, 0在整个Binder系统中有重要的含义
     * 因为0代表的是ServiceManager所对应的BBinder.
     *
     */
    return result; //返回BpBinder(handle)
}

wp<IBinder> ProcessState::getWeakProxyForHandle(int32_t handle)
{
    wp<IBinder> result;

    AutoMutex _l(mLock);

    handle_entry* e = lookupHandleLocked(handle);

    if (e != NULL) {        
        // We need to create a new BpBinder if there isn't currently one, OR we
        // are unable to acquire a weak reference on this current one.  The
        // attemptIncWeak() is safe because we know the BpBinder destructor will always
        // call expungeHandle(), which acquires the same lock we are holding now.
        // We need to do this because there is a race condition between someone
        // releasing a reference on this BpBinder, and a new reference on its handle
        // arriving from the driver.
        IBinder* b = e->binder;
        if (b == NULL || !e->refs->attemptIncWeak(this)) {
            b = new BpBinder(handle);
            result = b;
            e->binder = b;
            if (b) e->refs = b->getWeakRefs();
        } else {
            result = b;
            e->refs->decWeak(this);
        }
    }

    return result;
}

void ProcessState::expungeHandle(int32_t handle, IBinder* binder)
{
    AutoMutex _l(mLock);
    
    handle_entry* e = lookupHandleLocked(handle);

    // This handle may have already been replaced with a new BpBinder
    // (if someone failed the AttemptIncWeak() above); we don't want
    // to overwrite it.
    if (e && e->binder == binder) e->binder = NULL;
}

void ProcessState::setArgs(int argc, const char* const argv[])
{
    mArgC = argc;
    mArgV = (const char **)argv;

    mArgLen = 0;
    for (int i=0; i<argc; i++) {
        mArgLen += strlen(argv[i]) + 1;
    }
    mArgLen--;
}

int ProcessState::getArgC() const
{
    return mArgC;
}

const char* const* ProcessState::getArgV() const
{
    return mArgV;
}

void ProcessState::setArgV0(const char* txt)
{
    if (mArgV != NULL) {
        strncpy((char*)mArgV[0], txt, mArgLen);
        set_process_name(txt);
    }
}

void ProcessState::spawnPooledThread(bool isMain)
{
    if (mThreadPoolStarted) {
        int32_t s = android_atomic_add(1, &mThreadPoolSeq);
        char buf[16];
        snprintf(buf, sizeof(buf), "Binder_%X", s);
        ALOGV("Spawning new pooled thread, name=%s\n", buf);
        sp<Thread> t = new PoolThread(isMain);
        t->run(buf);
    }
}

status_t ProcessState::setThreadPoolMaxThreadCount(size_t maxThreads) {
    status_t result = NO_ERROR;
    if (ioctl(mDriverFD, BINDER_SET_MAX_THREADS, &maxThreads) == -1) {
        result = -errno;
        ALOGE("Binder ioctl to set max threads failed: %s", strerror(-result));
    }
    return result;
}

// 3. 打开binder设备
// open_driver的作用就是打开/dev/binder这个设备,
// 它是Android在内核中专门用于完成进程间通信而设置的一个虚拟设备
// 另外,由于ProcessState的唯一性,因此一个进程只打开binder设备一次
static int open_driver()
{
    int fd = open("/dev/binder", O_RDWR); //打开/dev/binder设备
    if (fd >= 0) {
        fcntl(fd, F_SETFD, FD_CLOEXEC);
        int vers;
        status_t result = ioctl(fd, BINDER_VERSION, &vers);
        if (result == -1) {
            ALOGE("Binder ioctl to obtain version failed: %s", strerror(errno));
            close(fd);
            fd = -1;
        }
        if (result != 0 || vers != BINDER_CURRENT_PROTOCOL_VERSION) {
            ALOGE("Binder driver protocol does not match user space protocol!");
            close(fd);
            fd = -1;
        }
        size_t maxThreads = 15;
        // 通过ioctl的方式告诉binder驱动,这个fd支持的最大线程数是15个. 
        result = ioctl(fd, BINDER_SET_MAX_THREADS, &maxThreads);
        if (result == -1) {
            ALOGE("Binder ioctl to set max threads failed: %s", strerror(errno));
        }
    } else {
        ALOGW("Opening '/dev/binder' failed: %s\n", strerror(errno));
    }
    return fd;
}

//2. ProcessState的构造
// 这个函数非常重要,它打开了Binder设备
ProcessState::ProcessState()
    // Android中有很多代码都是这么写的,稍不留神很容易忽略这里调用了一个很重要的函数
    : mDriverFD(open_driver())
    , mVMStart(MAP_FAILED) //映射内存的起始地址
    , mManagesContexts(false)
    , mBinderContextCheckFunc(NULL)
    , mBinderContextUserData(NULL)
    , mThreadPoolStarted(false)
    , mThreadPoolSeq(1)
{
    if (mDriverFD >= 0) {
        // XXX Ideally, there should be a specific define for whether we
        // have mmap (or whether we could possibly have the kernel module
        // availabla).
#if !defined(HAVE_WIN32_IPC)
        // mmap the binder, providing a chunk of virtual address space to receive transactions.
        // mmap的用法可以查看man.
        // 不过这个函数真正的实现和驱动有关系,而Binder驱动会分配一块内存来接收数据.
        mVMStart = mmap(0, BINDER_VM_SIZE, PROT_READ, MAP_PRIVATE | MAP_NORESERVE, mDriverFD, 0);
        if (mVMStart == MAP_FAILED) {
            // *sigh*
            ALOGE("Using /dev/binder failed: unable to mmap transaction memory.\n");
            close(mDriverFD);
            mDriverFD = -1;
        }
#else
        mDriverFD = -1;
#endif
    }

    LOG_ALWAYS_FATAL_IF(mDriverFD < 0, "Binder driver could not be opened.  Terminating.");
}

ProcessState::~ProcessState()
{
}
        
}; // namespace android
