#include<windows.h>
#include<iostream>

DWORD WINAPI mythread(LPVOID lpParameter)
{
	unsigned int& myCounter =*((unsigned int*)lpParameter);
	while(myCounter<0xFFFFFFFF) ++myCounter;
	return0;
}


int main(int argc,char* argv[])
{
	using namespace std;


	unsigned int myCounter=0;

	DWORD myThreadID;
	HANDLE myHandle =CreateThread(0,0,myThread,&myCounter;,0,&myThreadID);
	char myChar=' ';
	while(myChar!='q')
	{
		cout<< myCounter<<endl;
		myChar=getchar();
	}

	CloseHandle(myHandle);

	return 0;
}
