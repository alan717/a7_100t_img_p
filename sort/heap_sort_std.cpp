/************************************************
*
* File Name : heap_sort.cpp
* Purpose :
* Creation Date : 28-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	int myints[]={10,20,30,5,15};
	std::vector<int> v(myints,myints+5);

	std::make_heap(v.begin(),v.end());
	std::cout<< "initial max heap:" << v.front()<<'\n';

	std::pop_heap(v.begin(),v.end());v.pop_back();
	std::cout<< "max heap after pop:" <<v.front()<<'\n';

	v.push_back(99);std::push_heap()

}
