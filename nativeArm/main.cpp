#include <iostream>
#include<android/log.h>
#include "curl.h"
#define  LOG_TAG    "your-log-tag"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
 size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
    std::string *str = dynamic_cast<std::string *>((std::string *) lpVoid);
    if (NULL == str || NULL == buffer) {
        return -1;
    }

    char *pData = (char *) buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}
int main()
{
	 std::string tid="12350";
    char url[PATH_MAX];
    sprintf(url, "http://47.98.122.115:9090/update?game=dwrg&deviceId=%s", tid.c_str());
    LOGD("HearBeat send:%s", url);
    std::string strResponse;
    CURLcode res;
    CURL *curl = curl_easy_init();
    if (NULL == curl) {
        LOGE("初始化失败");
        return CURLE_FAILED_INIT;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &strResponse);
    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    LOGE("在线服务器返回%s",strResponse.c_str());

    return 0;
	return 0;
}
