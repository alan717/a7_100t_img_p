#include <iostream>
#include <tbb/tbb.h>
        
int main (int argc, char **argv)
{
	std::cout << "TBB version: " << TBB_VERSION_MAJOR
		  << "." << TBB_VERSION_MINOR << std::endl;
	return 0; 
}
