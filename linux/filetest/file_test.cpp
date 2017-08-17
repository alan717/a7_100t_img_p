/************************************************
*
* File Name : file_test.cpp
* Purpose :
* Creation Date : 18-08-2017
* Last Modified :
* Created By :
* Description :
*
************************************************/
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main()
{
    ifstream readfile;
    ofstream writefile;
    string filename = "myfile.txt";
    readfile.open( filename.c_str() );
    if( readfile )
    {
        //I can read something from the file
        readfile.close();
    }
    else
    {
        //The file doesn't exist
        writefile.open( filename.c_str() );
        //write something to the file
        writefile << " write some cpp data " <<std::endl;
        writefile.close();
    }
    return 0;
}
