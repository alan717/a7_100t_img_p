//#undef ENUM_LIST   
    
//#undef ENUM_NAME   
#define ENUM_NAME(...)  __VA_ARGS__   
    
    
//#undef ENUM_VALUE   
#define ENUM_VALUE(val) = val   
    
#define ENUM_COTENTS(...)  __VA_ARGS__   
    
#define DEFINE_ENUM(name)  enum name { ENUM_COTENTS(ENUM_LIST) };  
