/************************************************
*
* File Name : list_t.cpp
* Purpose :
* Creation Date : 19-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include <list>
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
int main()
{
	std::list<std::string> a_list;
	a_list.push_back("banana");
	a_list.push_front("apple");
	a_list.push_back("carrot");

	std::ostream_iterator<std::string> out_it(std::cout,"\n");
	std::copy(a_list.begin(),a_list.end(),
			out_it);
	std::reverse_copy(a_list.begin(),a_list.end(),
			out_it);
	std::copy(a_list.rbegin(),a_list.rend(),out_it);
	return 0;
}
