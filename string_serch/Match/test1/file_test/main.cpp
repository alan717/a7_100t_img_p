//#include
// basic file operations
#include "boost/lambda/lambda.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc ,char ** argv ) {
#ifdef filetest
	ofstream myfile;
	string line_str;
	/*
	myfile.open(argv[1],
		ios::app|
		ios::out|
		ios::binary);
	myfile << "Writing this to a file.\n";
	myfile.close();
	*/
	ifstream infile(argv[1]);
	// position
	ifstream::pos_type end, begin,cur;
	//infile.open(argv[1]);
	if (infile.is_open())
	{
		begin = infile.tellg();
		cur = infile.tellg();
		while (std::getline(infile, line_str))
		{	
			std::cout <<"file position:"<<cur<<" -- "<< line_str << std::endl;
			cur = infile.tellg();
		}
	}

	std::cout << "size is ::" << cur - begin << " bytes.\n" << std::endl;
	infile.close();
	// boost 1.67.0
#else
	using namespace boost::lambda;
	typedef std::istream_iterator<int> in;
	std::cout << "asdasda" << std::endl;
	std::for_each(
		in(std::cin), in(), std::cout << (_1 * 3) << " ");


	


#endif
	return 0;
}