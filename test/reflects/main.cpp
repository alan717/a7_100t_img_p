
#include<iostream>

#include "DefineEnum.h"
#include "RegisterEnum.h"
 
#define ENUM_LIST                                   \   
        ENUM_NAME(Sunday     ENUM_VALUE(10)),       \   
        ENUM_NAME(Monday     ENUM_VALUE(Sunday+1)),     \   
        ENUM_NAME(Tuesday    ENUM_VALUE(123)),      \   
        ENUM_NAME(Wednesday  ENUM_VALUE(10)) ,      \   
        ENUM_NAME(Thursday   ENUM_VALUE(7)),        \   
        ENUM_NAME(Friday     ENUM_VALUE(8)),        \   
        ENUM_NAME(Saturday   ENUM_VALUE(12))   
    
DEFINE_ENUM(WeekDay);   
    
#include "RegisterEnum.h"   
REGISTER_ENUM(WeekDay);   
    
    
int main()   
{   
    printf("%s is %d.", EnumHelper<WeekDay>::ToString(Monday), Monday);   
    getchar();   
    return 0;   
}  