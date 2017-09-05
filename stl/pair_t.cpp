/************************************************
*
* File Name : pair_t.cpp
* Purpose :
* Creation Date : 19-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include "pair_t.h"

template<typename T,typename U >
inline bool
operator == (const pair<T,U>& lhs,const pair<T,U>& rhs)
{
	return lhs.first == rhs.first && lhs.second == rhs.second;

}

template <typename T,typename U>
inline bool
operator < (const pair<T,U>& lhs,const pair<T,U>& rhs)
{
	return lhs.first<rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);

}
