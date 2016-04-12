/*
**
** Copyright 2008, The Android Open Source Project
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

#define LOG_TAG "mediaserver"
//#define LOG_NDEBUG 0
// System headers required for setgroups, etc.
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <linux/rtpm_prio.h>
#include <sys/prctl.h>
#include <private/android_filesystem_config.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

// from LOCAL_C_INCLUDES
#include "AudioFlinger.h"
#include "CameraService.h"
#include "MediaPlayerService.h"

#include "AudioPolicyService.h"

#ifndef ANDROID_DEFAULT_CODE
#include <memorydumper/MemoryDumper.h>
#endif

using namespace android;

int main(int argc, char** argv)
{
    signal(SIGPIPE, SIG_IGN);

    // ① 获得一个ProcessState实例,从名称来看是表示进程状态的对象
    sp<ProcessState> proc(ProcessState::self());

    // ② MediaServer作为ServiceManager的客户端,需要向ServiceManager注册服务
    // 调用defaultServiceManager,得到一个IServiceManager.
    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: %p", sm.get());
    
    // 初始化音频系统的AudioFliger服务
    AudioFlinger::instantiate();
    // ③ 初始化多媒体系统的MediaPlayer服务(这里以它为切入点)
    MediaPlayerService::instantiate();
#ifndef ANDROID_DEFAULT_CODE
    MemoryDumper::instantiate();
#endif
    // 初始化CameraService服务
    CameraService::instantiate();
    // 初始化音频系统的AudioPolicy服务
    AudioPolicyService::instantiate();

    if (AID_ROOT == getuid()) {
        ALOGI("[%s] re-adjust caps for its thread, and set uid to media", __func__);
        if (-1 == prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0)) {
            ALOGW("mediaserver prctl for set caps failed: %s", strerror(errno));
        } else {
            __user_cap_header_struct hdr;
            __user_cap_data_struct data;

            setuid(AID_MEDIA);         // change user to media
    
            hdr.version = _LINUX_CAPABILITY_VERSION;    // set caps again
            hdr.pid = 0;
            data.effective = (1 << CAP_SYS_NICE);
            data.permitted = (1 << CAP_SYS_NICE);
            data.inheritable = 0xffffffff;
            if (-1 == capset(&hdr, &data)) {
                ALOGW("mediaserver cap re-setting failed, %s", strerror(errno));
            }
        }

    } else {
        ALOGI("[%s] re-adjust caps is not in root user", __func__);
    }    

    // ④ 根据名称来推断,难道是要创建一个线程池吗？
    ProcessState::self()->startThreadPool();
    // ⑤ 下面的操作是要将自己加入到刚才的线程池中吗？
    IPCThreadState::self()->joinThreadPool();
}
