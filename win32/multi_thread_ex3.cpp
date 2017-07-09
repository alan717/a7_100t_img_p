#include<windows.h>
#include<process.h>
#include<stdio.h>


DWORD WINAPI mythreadA(__in LPVOID lpParameter)
{


	printf("CreateThread %d \n",GetCurrentThreadId());
	return 0;
}


unsigned int __stdcall mythreadB(void* data)
{
	printf("_beginthreadex %d \n",GetCurrentThreadId());
	return 0;
}

void mythreadC(void* data)
{
	printf("_beginthread %d \n" , GetCurrentThreadId());

}



int main(int argc ,char* argv[])
{

	HANDLE myhandleA,myhandleB,myhandleC;
	myhandleA=CreateThread(0,0,mythreadA,0,0,0);

	myhandleB=(HANDLE)_beginthreadex(0,0,&mythreadB,0,0,0);

	WaitForSingleObject(myhandleB,INFINITE);


	myhandlec=(HANDLE)_beginthread(&mythreadC,0,0);
	getchar();

	return 0 ;

}
