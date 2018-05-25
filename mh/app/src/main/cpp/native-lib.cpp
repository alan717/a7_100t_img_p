#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>

#define  TAG "fuck-mh"
#define LOGD(...)   __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

void __attribute__ ((constructor)) libhook_main() {
    LOGD("fuck mh:%s",getpid());

    char path[256],buff[256];

    snprintf(path, sizeof(path),"/proc/%d/smaps")


}
