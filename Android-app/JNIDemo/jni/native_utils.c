#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <android/log.h>
#define LOG_TAG    "NativeLog"
#define LOGV(...)    __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...)    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...)    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...)    __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...)    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


int GetCurrentPackageName(char *processName)
{
    int ret;
    char cmdline[256] = {0};

    sprintf(cmdline, "/proc/%d/cmdline", getpid());
    int fd = open(cmdline, O_RDONLY);
    if (fd == -1) {
        LOGE("open /proc/xxx/cmdline error");
        return -1;
    }

    ret = read(fd, processName, 128);
    if (ret == -1) {
        LOGE("read /proc/xxx/cmdline error");
        return -1;
    }

    close(fd);
    LOGD("current pid = %d , package name = %s", getpid(), processName);
    return 0;
}

jstring native_get_package_name(JNIEnv *env, jobject obj)
{
	char package_name[256];
	memset(package_name, 0, sizeof(package_name));
	GetCurrentPackageName(package_name);
	return (*env)->NewStringUTF(env, package_name);
}

jstring native_get_files_dir(JNIEnv *env, jobject obj)
{
    char package_name[256];
    memset(package_name, 0, sizeof(package_name));
    GetCurrentPackageName(package_name);
    char files_dir[512];
    sprintf(files_dir, "/data/data/%s/files", package_name);
    return (*env)->NewStringUTF(env, files_dir);
}

void native_get_files_dir_from_java(JNIEnv *env, jobject obj, jstring filesDir)
{
    const char *files_dir = (*env)->GetStringUTFChars(env, filesDir, NULL);
    LOGD("FileDir from Java: %s", files_dir);
    (*env)->ReleaseStringUTFChars(env, filesDir, files_dir);
}

static const JNINativeMethod methods[] = {
	{"nativeGetPackageName", "()Ljava/lang/String;", (void *)native_get_package_name},
    {"nativeGetFilesDir", "()Ljava/lang/String;", (void *)native_get_files_dir},
    {"nativeGetFilesDirFromJava", "(Ljava/lang/String;)V", (void *)native_get_files_dir_from_java}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;

    if((*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_6)) {
    	return JNI_ERR;
    }

    cls = (*env)->FindClass(env, "me/falcon/jnidemo/NativeTools");
    if(cls == NULL) {
    	return JNI_ERR;
    }

    if((*env)->RegisterNatives(env, cls, methods, 3) < 0) {
    	return JNI_ERR;	
    }
	
	return JNI_VERSION_1_6;
}