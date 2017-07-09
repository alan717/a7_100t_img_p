#include<windows.h>
#include<stdio.h>


DWORD WINAPI mythread(__in LPVOID lpParameter)
{

	printf("Thread inside %d \n",GetCurrentThreadId());
	return 0;

}

int main(int argc ,char* argv[])
{
	HANDLE myhandle;
	DWORD mythreadid;
	myhandle=CreateThread(0,0,mythread,0,0,&mythreadid;);
	printf("Thread after &d \n",mythreadid);
	getchar();
	return 0;

}
