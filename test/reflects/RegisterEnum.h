#include "ReflectEnum.h"   
    
#undef ENUM_VALUE   
#define ENUM_VALUE(val)   
    
#define REGISTER_ENUM(name)  REFLECT_ENUM(name, ENUM_LIST ) 