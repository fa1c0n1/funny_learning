LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := binder_server.c binder.c
LOCAL_MODULE := sh_binder_server
include $(BUILD_EXECUTABLE)




include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := binder_client.c binder.c
LOCAL_MODULE := sh_binder_client
include $(BUILD_EXECUTABLE)

