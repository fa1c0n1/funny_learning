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

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

// from LOCAL_C_INCLUDES
#include "AudioFlinger.h"
#include "CameraService.h"
#include "MediaPlayerService.h"
#include "AudioPolicyService.h"

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
    // 初始化CameraService服务
    CameraService::instantiate();
    // 初始化音频系统的AudioPolicy服务
    AudioPolicyService::instantiate();
    // ④ 根据名称来推断,难道是要创建一个线程池吗？
    ProcessState::self()->startThreadPool();
    // ⑤ 下面的操作是要将自己加入到刚才的线程池中吗？
    IPCThreadState::self()->joinThreadPool();
}
