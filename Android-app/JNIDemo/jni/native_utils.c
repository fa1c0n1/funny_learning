#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

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
    memset(files_dir, 0, sizeof(files_dir));
    sprintf(files_dir, "/data/data/%s/files", package_name);
    return (*env)->NewStringUTF(env, files_dir);
}

void native_get_files_dir_from_java(JNIEnv *env, jobject obj, jstring filesDir)
{
    const char *files_dir = (*env)->GetStringUTFChars(env, filesDir, NULL);
    LOGD("FileDir from Java: %s", files_dir);
    (*env)->ReleaseStringUTFChars(env, filesDir, files_dir);
}

jint native_sum_arr(JNIEnv *env, jobject obj, jintArray jarr)
{
    int len = (*env)->GetArrayLength(env, jarr);
    int carr[len];
    (*env)->GetIntArrayRegion(env, jarr, 0, len, carr);

    int i, sum = 0;
    for (i = 0; i < len; i++) {
        sum += carr[i];
    }

    LOGD("sum=%d", sum);
    return sum;
}

void native_get_sdcard_dir_from_java(JNIEnv *env, jobject obj, jstring sdcard)
{
    const char *sdcard_dir = (*env)->GetStringUTFChars(env, sdcard, NULL);
    LOGD("SDCard Dir from Java: %s", sdcard_dir);
    (*env)->ReleaseStringUTFChars(env, sdcard, sdcard_dir);
}

void native_create_file(JNIEnv *env, jobject obj, jstring fileName)
{
    const char *file_name = (*env)->GetStringUTFChars(env, fileName, NULL);
    char package_name[256];
    memset(package_name, 0, sizeof(package_name));
    GetCurrentPackageName(package_name);
    char file[512];
    memset(file, 0, sizeof(file));
    sprintf(file, "/data/data/%s/files/%s", package_name, file_name);
    LOGD("file=%s", file);
    int fd = open(file, O_CREAT | O_RDWR); //发现Android NDK的open函数指定mode无效，这里用fchmod函数指定mode
    if (fd < 0) {
        LOGE("open file error, errno=%d", errno);
        return ;
    }
    fchmod(fd, 0755);

    (*env)->ReleaseStringUTFChars(env, fileName, file_name);
}

static const JNINativeMethod methods[] = {
    {"nativeGetPackageName", "()Ljava/lang/String;", (void*)native_get_package_name},
    {"nativeGetFilesDir", "()Ljava/lang/String;", (void*)native_get_files_dir},
    {"nativeGetFilesDirFromJava", "(Ljava/lang/String;)V", (void*)native_get_files_dir_from_java},
    {"nativeSumArray", "([I)I", (void*)native_sum_arr},
    {"nativeGetSDCardDirFromJava", "(Ljava/lang/String;)V", (void*)native_get_sdcard_dir_from_java},
    {"nativeCreateFile", "(Ljava/lang/String;)V", (void*)native_create_file}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reversed)
{
    JNIEnv *env;
    jclass cls;

    if ((*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_6)) {
        return JNI_ERR;
    }

    cls = (*env)->FindClass(env, "me/falcon/jnidemo/NativeTools");
    if (cls == NULL) {
        return JNI_ERR;
    }

    if ((*env)->RegisterNatives(env, cls, methods, 6) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
