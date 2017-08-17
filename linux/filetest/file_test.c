/************************************************
*
* File Name : file_test.cpp
* Purpose :
* Creation Date : 18-08-2017
* Last Modified : Fri 18 Aug 2017 12:11:10 AM HKT
* Created By :
* Description :
*
************************************************/
#include<stdio.h>

int main()
{
    FILE *read;
    FILE *write;
    char *filename = "myfile.txt";
    read = fopen( filename, "r" );
    if( read == NULL )
    {
        //File doesn't exist. Have to write something
        write = fopen( filename, "w" );
        fprintf( write, "%s", "write c language  something here" );
        fclose( write );
    }
    else
    {
        //File exists. Read data from the file
        char somedata[ 20 ];
        fscanf( read, "%s", somedata );
        printf( "%s", somedata );
        fclose( read );
    }
    return 0;
}
