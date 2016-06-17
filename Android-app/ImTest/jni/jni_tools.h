/** 
 * Copyright (c) 2015 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */
#include <jni.h>
#include <stdio.h>
#include <Android/log.h>

#ifndef JNI_TOOLS_H
#define JNI_TOOLS_H

#define TAG "LaiXou" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型
#ifdef __cplusplus
extern "C" {
#endif

jmethodID GetMethodId(JNIEnv *env, jclass cls, char *key, char *signature);

jclass GetObjectClass(JNIEnv *env, jobject obj);

char *jstringToCharArr(JNIEnv *env, jstring string);
jstring charArrToJstring(JNIEnv *env, const char *pat);
char *getStringFromObj(JNIEnv *env, jobject obj, jclass objClass, char *key);

int getIntFromObj(JNIEnv *env, jobject obj, jclass objClass, char *key);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
