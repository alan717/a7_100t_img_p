//
// Created by think on 2018/5/11.
//

#ifndef PLEASELETMELOADTHISLIBRARY_UTIL_H
#define PLEASELETMELOADTHISLIBRARY_UTIL_H
#include <android/log.h>
#define  LOG_TAG    "luaTag"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif //PLEASELETMELOADTHISLIBRARY_UTIL_H
