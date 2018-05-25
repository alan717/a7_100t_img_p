//
// Created by think on 2018/5/9.
//
#include <lua.hpp>
#include <iostream>


#include "luaImportList.h"
#include "util.h"


#define  VER        "V0.0.1"

//todo:方便注册自己的函数
#define REGLUAFUN(z, x, y) \
    lua_register(z, x, y); \
    LOGD("[lua]注册函数 %s", x);

int luacallxx(const char *str);

int lualog(lua_State *luastate);

lua_State *L;

int main() {
/*    //这些代码是做测试用的
    LOGI("LUA热更启动!\nver: %s\nbuild time:%s %s", VER, __DATE__, __TIME__);
    // lua_State* L=luaL_newstate();
    // luaL_openlibs(L);
    // lua_register(L,"clickPic",clickPic);

    L = luaL_newstate();
    luaL_openlibs(L);

    //注册函数
    lua_register(L, "log", lualog);
    REGLUAFUN(L,"httpPost",httpPost);
    REGLUAFUN(L,"httpGet",httpGet);
    //測試命令
    REGLUAFUN(L,"shellcmd",shellcmd);

    luaL_dofile(L, "/sdcard/test.lua");

    //luacallxx("666");
    lua_close(L);*/
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

}

int luacallxx(const char *str) {
    lua_getglobal(L, "test");
    lua_pushstring(L, str);
    lua_call(L, 1, 1);
    std::string ss = lua_tostring(L, -1);
    lua_pop(L, 1);
    std::cout << ss.c_str() << std::endl;
    LOGI("GET LUA STRING %s", ss.c_str());
    return 0;
}

int lualog(lua_State *luastate) {
    int nargs = lua_gettop(luastate);
    std::string t;
    LOGI("%d", nargs);
    for (int i = 1; i <= nargs; i++) {
        if (lua_istable(luastate, i))
            t += "table";
        else if (lua_isnone(luastate, i))
            t += "none";
        else if (lua_isnil(luastate, i))
            t += "nil";
        else if (lua_isboolean(luastate, i)) {
            if (lua_toboolean(luastate, i) != 0)
                t += "true";
            else
                t += "false";
        } else if (lua_isfunction(luastate, i))
            t += "function";
        else if (lua_islightuserdata(luastate, i))
            t += "lightuserdata";
        else if (lua_isthread(luastate, i))
            t += "thread";
        else {
            const char *str = lua_tostring(luastate, i);
            if (str)
                t += lua_tostring(luastate, i);
            else
                t += lua_typename(luastate, lua_type(luastate, i));
        }
        if (i != nargs)
            t += "\t";
    }
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_ERROR,
                        "lua call cpp debug info", "%s", t.c_str());
#else
    CCLOG("[LUA-print] %s", t.c_str());
#endif
    return 0;
}
