//
// Created by beijiuhu on 3/26/2018.
//


#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>

#include <lua.h>
#include <lapi.h>

#include "ClickHelper.h"



extern "C"
JNIEXPORT void JNICALL
Java_com_lyg_beijiuhu_pleaseletmeloadthisso_Screen_clickPic(JNIEnv *env, jclass type, jstring picPath_) {
    const char *picPath = env->GetStringUTFChars(picPath_, 0);

    // TODO
    if (strstr(picPath, "_")) {
        ClickHelper::clickPic2(picPath);
    }
    else {
        ClickHelper::clickPic(picPath);
    }

    env->ReleaseStringUTFChars(picPath_, picPath);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lyg_beijiuhu_pleaseletmeloadthisso_Screen_clickPoint(JNIEnv *env, jclass type, jint x, jint y) {

    // TODO
    ClickHelper::clickPoint(x, y);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_lyg_beijiuhu_pleaseletmeloadthisso_Screen_getPoint(JNIEnv *env, jclass type, jstring picPath_) {
    const char *picPath = env->GetStringUTFChars(picPath_, 0);

    // TODO

    cv::Point point;
    bool r = ClickHelper::getPoint(picPath, point);
    if (r) {
        jclass clazz = env->FindClass("android/graphics/Point");

        // Get the method id of an empty constructor in clazz
        jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");

        // Create an instance of clazz
        jobject obj = env->NewObject(clazz, constructor);

        jfieldID fx = env->GetFieldID(clazz, "x", "I");
        jfieldID fy = env->GetFieldID(clazz, "y", "I");

        // Set fields for object
        env->SetIntField(obj, fx, point.x);
        env->SetIntField(obj, fy, point.y);

        env->ReleaseStringUTFChars(picPath_, picPath);
        return obj;
    }

    env->ReleaseStringUTFChars(picPath_, picPath);
    return NULL;
}

