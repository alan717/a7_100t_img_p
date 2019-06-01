#ifndef REFLECT_ENUM_INCLUDE_GUARD   

#include <string>   
#include <cstring>   
#include <stdexcept> // for runtime_error   

#endif   

template <typename Enum_T> class EnumHelper
{
public:
	static const char * ToString(Enum_T e)
	{
		for (int i = 0; i < _countof(EnumHelper<Enum_T>::s_allEnums); i++)
		{
			if (s_allEnums[i] == e)
				return s_allEnumNames[i];
		}
		return NULL;
	}

private:
	static const char*				s_typeName;
	static Enum_T					s_allEnums[];
	static char						s_singleEnumStr[];
	static const char*				s_allEnumNames[];

	static void SplitEnumDefString()
	{
		char * p = s_singleEnumStr;
		while (isspace(*p)) p++;
		for (int i = 0; i < _countof(EnumHelper<Enum_T>::s_allEnums); i++)
		{
			s_allEnumNames[i] = p;
			while (*p == '_' || isdigit(*p) || isalpha(*p)) p++;
			bool meet_comma = (*p == ',');
			*p++ = '\0';
			if (!meet_comma)
			{
				while (*p && *p != ',') p++;
				if (*p) p++;
			}
			while (*p && isspace(*p)) p++;
		}
	}
};

#define TO_ENUM_ITEM(...)  __VA_ARGS__   
#define STRINGIZE(...)  #__VA_ARGS__   

#define REFLECT_ENUM(enum_type_name, enum_list)																			\
template<> enum_type_name EnumHelper<enum_type_name>::s_allEnums[]=														\
{																														\
    TO_ENUM_ITEM(enum_list)                                                                                             \
};                                                                                                                      \
template<> const char* EnumHelper<enum_type_name>::s_allEnumNames[_countof(EnumHelper<enum_type_name>::s_allEnums)];	\
template<> char EnumHelper<enum_type_name>::s_singleEnumStr[] = STRINGIZE(enum_list) ;                                  \
template<> const char * EnumHelper<enum_type_name>::s_typeName = (EnumHelper<enum_type_name>::SplitEnumDefString(), #enum_type_name);