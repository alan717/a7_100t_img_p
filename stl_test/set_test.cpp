#include <iostream>
#include <iterator>
#include <set>
#include <cassert>

int main()
{
	std::set<int> myset;
	for(int i=1;i<=5;i++)
	{
		myset.insert(i*10);
	}
	std::pair<std::set<int>::iterator ,bool> ret=myset.insert(20);
	assert(ret.second== false);
	
	int myints[]={5,10,15};
	myset.insert(myints,myints+3);
	
	std::copy(myset.begin(),myset.end(),
	std::ostream_iterator<int> (std::cout,"\n"));
	return 0;


}
