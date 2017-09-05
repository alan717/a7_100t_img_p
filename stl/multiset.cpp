/************************************************
*
* File Name : multiset.cpp
* Purpose :
* Creation Date : 20-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/

#include <set>
#include <iostream>
#include <iterator>

int main()
{
	const int N= 10;
	int a[N]={4,1,1,1,1,0,5,1,0,1};
	int b[N]={4,4,2,5,4,0,1,5,5,1};

	std::multiset<int> A(a,a+N);
	std::multiset<int> B(b,b+N);
	std::multiset<int> C;
	std::cout << "set A:";
	std::copy(A.begin(),A.end(),std::ostream_iterator<int>(std::cout," "));
	std::cout<< std::endl;

	std::cout<<"Set B:";
	std::copy(B.begin(),B.end(),std::ostream_iterator<int>(std::cout," "));

	std::cout<< std::endl;
	std::cout << "Union:";
	std::set_union(A.begin(),A.end(),B.begin(),B.end(),std::ostream_iterator<int>(std::cout," "));
	std::cout <<std::endl;

	std::cout<<"intersection:";



}
