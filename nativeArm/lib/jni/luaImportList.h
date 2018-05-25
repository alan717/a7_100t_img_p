//
// Created by think on 2018/5/10.
//

#ifndef PLEASELETMELOADTHISLIBRARY_CPP_CALLTEST_H
#define PLEASELETMELOADTHISLIBRARY_CPP_CALLTEST_H
#ifdef __cplusplus

extern  "C"{
    #include <lua.h>
#endif
    //图片点击
    int clickPic(lua_State*);
    int shellcmd(lua_State*);
    int httpPost(lua_State*);
    int httpGet(lua_State*);
    int getPicPos(lua_State*);
    int luacallxx(const char *str);
    int lualog(lua_State *luastate);


#ifdef __cplusplus
};
#endif
#endif //PLEASELETMELOADTHISLIBRARY_CPP_CALLTEST_H
