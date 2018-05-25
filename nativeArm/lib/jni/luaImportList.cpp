//
// Created by think on 2018/5/10.
//
#include "luaImportList.h"

#ifdef __cplusplus

#include "lua.hpp"
#include "ClickHelper.h"
#include <string>
#include "http.h"

extern "C" {
#endif
int clickPic(lua_State *L) {
    const char *picPath = luaL_checklstring(L, 1, nullptr);
    if (strstr(picPath, "_")) {
        ClickHelper::clickPic2(picPath);
    } else {
        ClickHelper::clickPic(picPath);
    }

    return 0;
}

int shellcmd(lua_State *L) {
    const char *cmd = luaL_checklstring(L, 1, nullptr);
    int ret=system(cmd);
    lua_pushnumber(L,ret);

    //LOGI(cmd);
    return 1;
}

int httpPost(lua_State *L) {
    LOGI("HTTP_POST");
    const char *url = luaL_checklstring(L, 1, nullptr);
    const char *postdata = luaL_checklstring(L, 2, nullptr);
    LOGI("postData:%s", postdata);
    std::string resp;
    CHttpClient client;
    client.Post(url, postdata, resp);
    LOGI("resp:%s", resp.c_str());
    lua_pushstring(L, resp.c_str());
    return 1;
}

int httpGet(lua_State *L) {
    LOGI("HTTP_GET");
    const char *url = luaL_checklstring(L, 1, nullptr);
    CHttpClient client;
    std::string resp;
    client.Get(url, resp);
    lua_pushstring(L, resp.c_str());
    return 1;
}

int getPicPos(lua_State* L){
    LOGI("get pic position");
    const char *picpath=luaL_checklstring(L,1, nullptr);
    double  r;
    int a,b;
    ClickHelper::matchpic(picpath,r,a,b);
    //lua_pushnumber(L,);
    

}

#ifdef __cplusplus

}

#endif




