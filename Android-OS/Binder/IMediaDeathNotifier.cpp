/*
** Copyright 2010, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "IMediaDeathNotifier"
#include <utils/Log.h>

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <media/IMediaDeathNotifier.h>

namespace android {

// client singleton for binder interface to services
Mutex IMediaDeathNotifier::sServiceLock;
sp<IMediaPlayerService> IMediaDeathNotifier::sMediaPlayerService;
sp<IMediaDeathNotifier::DeathNotifier> IMediaDeathNotifier::sDeathNotifier;
SortedVector< wp<IMediaDeathNotifier> > IMediaDeathNotifier::sObitRecipients;

/* 
 * 前面分析ServiceManager的作用可知,一个Client想要得到某个Service的信息,
 * 就必须和ServiceManager打交道. 
 * 通过调用BpServiceManager的getService()函数来获取对应Service的信息.
 *
 * 请看来源于IMediaDeathNotifier.cpp中的例子 getMediaPlayerService()
 *
 * 这个函数通过与ServiceManager通信,获得一个能够与MediaPlayerService通信的BpBinder
 *
 */
// establish binder interface to MediaPlayerService
/*static*/const sp<IMediaPlayerService>&
IMediaDeathNotifier::getMediaPlayerService()
{
    ALOGV("getMediaPlayerService");
    Mutex::Autolock _l(sServiceLock);
    if (sMediaPlayerService == 0) {
        //得到BpServiceManager对象,handle为0，所以目的端是ServiceManager
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;
        do {
            //通过BpServiceManager的getService()函数,
            //  向ServiceManager查询服务的信息,
            //  返回一个可以与MediaPlayerService通信的BpBinder,
            //  且该BpBinder中的handle标识的一定是MediaPlayerService
            binder = sm->getService(String16("media.player"));
            if (binder != 0) {
                break;
            }
            ALOGW("Media player service not published, waiting...");

            //如果ServiceManager上还没有注册对应的服务,则需要等待,
            //直到对应的服务被注册
            usleep(500000); // 0.5 s
        } while (true);

        if (sDeathNotifier == NULL) {
        sDeathNotifier = new DeathNotifier();
    }
    //调用下面这个函数,告诉系统我们对这个binder的生死有兴趣,
    //这个binder是一个BpBinder,它关心的是对端BBinder,即BnXXXService的父类
    binder->linkToDeath(sDeathNotifier);

    /*再通过障眼法般的模版函数interface_cast将上面返回的
     *  BpBinder转换成BpMediaPlayerService对象.
     *  
     *有了BpMediaPlayerService对象,就能使用任何IMediaPlayerService提供的业务函数了.
     *  例如: createMetadataRetriever, createMediaRecorder等.
     *
     *显而易见的是,
     *   调用的这些函数都将把请求数据打包发送给Binder驱动,由BpBinder中的handle值找到对应目的端处理者来处理.
     *   其中的过程如下:
     *      (1) 通讯层接收到请求
     *      (2) 递交给业务层处理
     */
    sMediaPlayerService = interface_cast<IMediaPlayerService>(binder);
    }
    ALOGE_IF(sMediaPlayerService == 0, "no media player service!?");
    return sMediaPlayerService;
}

/*static*/ void
IMediaDeathNotifier::addObitRecipient(const wp<IMediaDeathNotifier>& recipient)
{
    ALOGV("addObitRecipient");
    Mutex::Autolock _l(sServiceLock);
    sObitRecipients.add(recipient);
}

/*static*/ void
IMediaDeathNotifier::removeObitRecipient(const wp<IMediaDeathNotifier>& recipient)
{
    ALOGV("removeObitRecipient");
    Mutex::Autolock _l(sServiceLock);
    sObitRecipients.remove(recipient);
}

void
IMediaDeathNotifier::DeathNotifier::binderDied(const wp<IBinder>& who) {
    ALOGW("media server died");

    // Need to do this with the lock held
    SortedVector< wp<IMediaDeathNotifier> > list;
    {
        Mutex::Autolock _l(sServiceLock);
        //将自己保存的BpMediaPlayerService对象干掉
        sMediaPlayerService.clear();
        list = sObitRecipients;
    }

    // Notify application when media server dies.
    // Don't hold the static lock during callback in case app
    // makes a call that needs the lock.
    size_t count = list.size();
    for (size_t iter = 0; iter < count; ++iter) {
        sp<IMediaDeathNotifier> notifier = list[iter].promote();
        if (notifier != 0) {
            notifier->died();
        }
    }
}

IMediaDeathNotifier::DeathNotifier::~DeathNotifier()
{
    ALOGV("DeathNotifier::~DeathNotifier");
    Mutex::Autolock _l(sServiceLock);
    sObitRecipients.clear();
    if (sMediaPlayerService != 0) {
        //unlinkToDeath调用可以取消对BnMediaPlayerService的关心。
        /*
         * 场景：
         *      DeathNotifier对象不想活了,但是BnMediaPlayerService还活着.
         *      或者DeathNotifier中途变卦. 怎么办?
         *
         *   Binder的这个讣告,想知道它是如何做到的,可以去看看驱动的实现.
         */
        sMediaPlayerService->asBinder()->unlinkToDeath(this);
    }
}

}; // namespace android
