LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := rpc_server.c binder.c
LOCAL_MODULE := rpc_server
include $(BUILD_EXECUTABLE)




include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SRC_FILES := rpc_client.c binder.c
LOCAL_MODULE := rpc_client
include $(BUILD_EXECUTABLE)

