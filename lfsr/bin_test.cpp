#include <fstream>
#include <iostream>


using namespace std;

bool XOR(bool In1,bool In2)
{
	return ((!In1&&In2)||(In1&&In2));	
};

int main()
{
	ofstream out("bin_lfsr_gen.txt",ios::out);
	bool *LFSR=new bool[24];
	for(int i=0;i<24;i++)
	{
		LFSR[i]=1;
	}
	


	for(int j=0;j<1024;j++)
	{
		for()
	}




}
