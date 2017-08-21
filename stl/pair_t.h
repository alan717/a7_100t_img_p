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
template <typename T,typename U>
struct pair
{
	//data members
	T first;
	U second;

	//default construtor 
	pair(){}

	//constructor from values
	pair(const T& t,const U& u):first(t),second(u){}
};
