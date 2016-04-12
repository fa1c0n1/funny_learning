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

#define LOG_TAG "ServiceManager"

#include <binder/IServiceManager.h>

#include <utils/Debug.h>
#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <binder/Parcel.h>
#include <utils/String8.h>
#include <utils/SystemClock.h>

#include <private/binder/Static.h>

#include <unistd.h>

namespace android {

sp<IServiceManager> defaultServiceManager()
{
    // 又是一个单例(Singleton)
    if (gDefaultServiceManager != NULL) return gDefaultServiceManager;
    
    {
        AutoMutex _l(gDefaultServiceManagerLock);
        if (gDefaultServiceManager == NULL) {
            // 通过源码分析,interface_cast其实是一个障眼法
            /* 通过代码的跟进分析,interface_cast不是指针的转换,而是利用
             *  BpBinder对象作为参数新建了一个BpServiceManager对象
             */
            gDefaultServiceManager = interface_cast<IServiceManager>(
                ProcessState::self()->getContextObject(NULL));
            //通过源码分析可知,上面这句代码相当于：
            // gDefaultServiceManager = interface_cast<IServiceManager>(new BpBinder(0));
        }
    }
    
    return gDefaultServiceManager;
}

bool checkCallingPermission(const String16& permission)
{
    return checkCallingPermission(permission, NULL, NULL);
}

static String16 _permission("permission");


bool checkCallingPermission(const String16& permission, int32_t* outPid, int32_t* outUid)
{
    IPCThreadState* ipcState = IPCThreadState::self();
    pid_t pid = ipcState->getCallingPid();
    uid_t uid = ipcState->getCallingUid();
    if (outPid) *outPid = pid;
    if (outUid) *outUid = uid;
    return checkPermission(permission, pid, uid);
}

bool checkPermission(const String16& permission, pid_t pid, uid_t uid)
{
    sp<IPermissionController> pc;
    gDefaultServiceManagerLock.lock();
    pc = gPermissionController;
    gDefaultServiceManagerLock.unlock();
    
    int64_t startTime = 0;

    while (true) {
        if (pc != NULL) {
            bool res = pc->checkPermission(permission, pid, uid);
            if (res) {
                if (startTime != 0) {
                    ALOGI("Check passed after %d seconds for %s from uid=%d pid=%d",
                            (int)((uptimeMillis()-startTime)/1000),
                            String8(permission).string(), uid, pid);
                }
                return res;
            }
            
            // Is this a permission failure, or did the controller go away?
            if (pc->asBinder()->isBinderAlive()) {
                ALOGW("Permission failure: %s from uid=%d pid=%d",
                        String8(permission).string(), uid, pid);
                return false;
            }
            
            // Object is dead!
            gDefaultServiceManagerLock.lock();
            if (gPermissionController == pc) {
                gPermissionController = NULL;
            }
            gDefaultServiceManagerLock.unlock();
        }
    
        // Need to retrieve the permission controller.
        sp<IBinder> binder = defaultServiceManager()->checkService(_permission);
        if (binder == NULL) {
            // Wait for the permission controller to come back...
            if (startTime == 0) {
                startTime = uptimeMillis();
                ALOGI("Waiting to check permission %s from uid=%d pid=%d",
                        String8(permission).string(), uid, pid);
            }
            sleep(1);
        } else {
            pc = interface_cast<IPermissionController>(binder);
            // Install the new permission controller, and try again.        
            gDefaultServiceManagerLock.lock();
            gPermissionController = pc;
            gDefaultServiceManagerLock.unlock();
        }
    }
}

// ----------------------------------------------------------------------

// BpServiceManager对象实现了IServiceManager所声明的业务函数:
// getService, checkService, addService, listServices
class BpServiceManager : public BpInterface<IServiceManager>
{
public:
    BpServiceManager(const sp<IBinder>& impl)
        : BpInterface<IServiceManager>(impl)
    {
    }

    virtual sp<IBinder> getService(const String16& name) const
    {
        unsigned n;
        for (n = 0; n < 5; n++){
            sp<IBinder> svc = checkService(name);
            if (svc != NULL) return svc;
            ALOGI("Waiting for service %s...\n", String8(name).string());
            sleep(1);
        }
        return NULL;
    }

    // 查询服务
    virtual sp<IBinder> checkService( const String16& name) const
    {
        Parcel data, reply;
        data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
        data.writeString16(name);
        remote()->transact(CHECK_SERVICE_TRANSACTION, data, &reply);
        return reply.readStrongBinder();
    }

    // 注册服务
    virtual status_t addService(const String16& name, const sp<IBinder>& service,
            bool allowIsolated)
    {
        // Parcel: 就把它当作是一个数据包
        Parcel data, reply;
        data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
        data.writeString16(name);
        data.writeStrongBinder(service);
        data.writeInt32(allowIsolated ? 1 : 0);
        /* remote()返回的是mRemote,也就是BpBinder对象,
         * 然后将上面封装好的请求数据包data,传入BpBinder的transact()方法
         * 至此，业务层的工作原理就清晰了，它的作用就是将请求信息打包后,
         * 再交给通信层去处理
         */
        status_t err = remote()->transact(ADD_SERVICE_TRANSACTION, data, &reply);
        return err == NO_ERROR ? reply.readExceptionCode() : err;
    }

    virtual Vector<String16> listServices()
    {
        Vector<String16> res;
        int n = 0;

        for (;;) {
            Parcel data, reply;
            data.writeInterfaceToken(IServiceManager::getInterfaceDescriptor());
            data.writeInt32(n++);
            status_t err = remote()->transact(LIST_SERVICES_TRANSACTION, data, &reply);
            if (err != NO_ERROR)
                break;
            res.add(reply.readString16());
        }
        return res;
    }
};

// IMPLEMENT_META_INTERFACE 这个宏非常关键！
/*
 * IMPLEMENT_META_INTERFACE(ServiceManager, "android.os.IServiceManager")进行宏替换可得:
 *
 *     const android::String16 IServiceManager::descriptor("android.os.IServiceManager");
 *     const android::String16& IServiceManager::getInterfaceDescriptor() const {
 *          return IServiceManager::descriptor; 
 *     }
 *     android::sp<IServiceManager> IServiceManager::asInterface(const android::sp<android::IBinder>& obj)
 *     {
 *          android::sp<IServiceManager> intr;
 *          if (obj != NULL) {
 *              intr = static_cast<IServiceManager*>(obj->queryLocalInterface(IServiceManager::descriptor).get());
 *              if (intr == NULL) {
 *                  // 这就是defaultServiceManager函数中,
 *                  // interface_cast把BpBinder指针转换成一个IServiceManager指针的原因
 *                  intr = new BpServiceManager(obj);
 *              }
 *          }
 *          return intr;
 *     }
 *     IServiceManager::IServiceManager() { }
 *     IServiceManager::~IServiceManager() { }
 *
 */
IMPLEMENT_META_INTERFACE(ServiceManager, "android.os.IServiceManager");

// ----------------------------------------------------------------------

status_t BnServiceManager::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    //printf("ServiceManager received: "); data.print();
    switch(code) {
        case GET_SERVICE_TRANSACTION: {
            CHECK_INTERFACE(IServiceManager, data, reply);
            String16 which = data.readString16();
            sp<IBinder> b = const_cast<BnServiceManager*>(this)->getService(which);
            reply->writeStrongBinder(b);
            return NO_ERROR;
        } break;
        case CHECK_SERVICE_TRANSACTION: {
            CHECK_INTERFACE(IServiceManager, data, reply);
            String16 which = data.readString16();
            sp<IBinder> b = const_cast<BnServiceManager*>(this)->checkService(which);
            reply->writeStrongBinder(b);
            return NO_ERROR;
        } break;
        case ADD_SERVICE_TRANSACTION: {
            CHECK_INTERFACE(IServiceManager, data, reply);
            String16 which = data.readString16();
            sp<IBinder> b = data.readStrongBinder();
            status_t err = addService(which, b);
            reply->writeInt32(err);
            return NO_ERROR;
        } break;
        case LIST_SERVICES_TRANSACTION: {
            CHECK_INTERFACE(IServiceManager, data, reply);
            Vector<String16> list = listServices();
            const size_t N = list.size();
            reply->writeInt32(N);
            for (size_t i=0; i<N; i++) {
                reply->writeString16(list[i]);
            }
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace android
