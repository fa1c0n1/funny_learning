LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE 	:= jnidemo 
LOCAL_SRC_FILES	:= native_utils.c 
LOCAL_LDFLAGS	+= -L$(LOCAL_PATH)
LOCAL_LDLIBS	:= -llog

include $(BUILD_SHARED_LIBRARY)