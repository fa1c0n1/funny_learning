/** 
 * Copyright (c) 2015 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <string.h>
#include "jni_tools.h"
/**
 *获取方法的id
 * key=方法名
 * signature=方法签名,例如,参数为整数,返回值为void ="(I)V"
 */
jmethodID GetMethodId(JNIEnv *env, jclass cls, char *key, char *signature) {
    return (*env)->GetMethodID(env, cls, key, signature);
}

jclass GetObjectClass(JNIEnv *env, jobject obj) {
    return (*env)->GetObjectClass(env, obj);
}

jstring charArrToJstring(JNIEnv *env, const char *pat) {
//定义java String类 strClass
    jclass strClass = (*env)->FindClass(env, "Ljava/lang/String;");
    LOGD("获取String类");
//获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
    LOGD("获取String构造方法");
//建立byte数组
    jbyteArray bytes = (*env)->NewByteArray(env, (jsize) strlen(pat));
    LOGD("建立数组");
//将char* 转换为byte数组
    (*env)->SetByteArrayRegion(env, bytes, 0, (jsize) strlen(pat), (jbyte *) pat);
    LOGD("转成byte数组");
//设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
    LOGD("设置utf-8");
    //将byte数组转换为java String,并输出
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

char *jstringToCharArr(JNIEnv *env, jstring str) {
    return (*env)->GetStringUTFChars(env, str, NULL);
}

char *getStringFromObj(JNIEnv *env, jobject obj, jclass objClass, char *key) {
    jfieldID nameFieldId;
    nameFieldId = (*env)->GetFieldID(env, objClass, key, "Ljava/lang/String;"); //获得属性句柄
    if (nameFieldId == NULL) {
        return NULL;
    }
    jstring javaNameStr = (jstring) (*env)->GetObjectField(env, obj, nameFieldId);  // 获得该属性的值
    const char *c_javaName = (*env)->GetStringUTFChars(env, javaNameStr, NULL);  //转换为 char *类型
    (*env)->ReleaseStringUTFChars(env, javaNameStr, c_javaName);  //释放局部引用*/
    return c_javaName;
}

int getIntFromObj(JNIEnv *env, jobject obj, jclass objClass, char *key) {
    int intValue = -1;
    jfieldID nameFieldId;
    nameFieldId = (*env)->GetFieldID(env, objClass, key, "I"); //获得属性句柄
    if (nameFieldId == NULL) {
        return -1;
    }
    intValue = (int) (*env)->GetIntField(env, obj, nameFieldId);  // 获得该属性的值
    return intValue;
}


