#include <iostream>

#include <winsock2.h>
#include <windows.h>
#include "data_def.h"
int main()
{


	protocol_def_pack sss;
	sss.uid = static_cast<BYTE>('a');
	sss.alph = 'A';
	sss.data = 100;
	protocol_def_unpack ppp;
	ppp.uid = static_cast<BYTE>('B');
	ppp.alph = 'B';
	ppp.data = 200;
	//std::cout.write("",);
	std::cout << "pack_size:" << sizeof(protocol_def_pack) << ",\nunpck_size:" << sizeof(protocol_def_unpack) << '\n';
    return 0;
}
