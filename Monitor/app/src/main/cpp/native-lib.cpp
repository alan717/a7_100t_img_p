#include <jni.h>
#include <string>

#include "Worker.h"

extern "C" JNIEXPORT jstring




JNICALL
Java_com_example_user567_monitor_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}


void process()
{
    return 0;
}
