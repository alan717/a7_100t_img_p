#include <iostream>
#include "ElfReader.h"
using namespace std;

int main() {
#ifndef BPROC
    cout << "Please set BPROC option to build!!!" << endl ;
    return 0;
#endif

    ElfReader reader;
    reader.parseFromFile("C:\\Users\\pwd61\\Desktop\\libjdbitmapkit.so");
	if (reader.readIdent()) cout << "ident"<<endl;
    reader.readHeader();
    reader.readProgramheader();
    reader.readSectionHeader();
    cout << "Hello, World!" << endl;
    return 0;
}