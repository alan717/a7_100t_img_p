/************************************************
*
* File Name : vec.cpp
* Purpose :
* Creation Date : 19-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include <iostream>
#include <vector>
#include <string>

int main(int argc,char* argv[])
{
	std::vector<std::string> projects;

	std::cout<<"program name:"
		<<argv[0]<<std::endl;

	for(int i=1;i < argc ;++i)
	{
		projects.push_back(argv[i]);
		std::cout<< projects[i-1]
			<<std::endl;

	}

	return 0;
}
