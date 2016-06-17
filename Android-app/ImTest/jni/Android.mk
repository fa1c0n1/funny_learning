LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := imSDK
LOCAL_LDFLAGS	+= -L$(LOCAL_PATH)
LOCAL_LDLIBS := \
	-llog \

LOCAL_SRC_FILES := \
	dyad.c \
	imsdk.c \
	imsdk_jni.c \
	jni_tools.c \
	parson.c


include $(BUILD_SHARED_LIBRARY)









