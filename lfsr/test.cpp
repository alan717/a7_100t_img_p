#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	uint16_t start_state=0xACE1u;
	uint16_t lfsr= start_state;
	uint16_t bit;
	unsigned period=0;
	ofstream out("lfsr_gen.txt",ios::out);
	do
	{
		bit=((lfsr>>0)^(lfsr>>2)^(lfsr>>3)^ (lfsr >> 5))&1;
		lfsr=(lfsr>>1) | (bit << 15);
		++period;
//		std::cout<<lfsr<<endl;
		out<<lfsr<<endl;
		
	}while(lfsr!=start_state);
	std::cout<<period<<endl;

	return 0;
}
