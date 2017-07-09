#include <Windows.h>
#include <process.h>
#include <stdio.h>

unsigned int __stdcall mythread(void* data) 
{
	printf("Thread %d\n", GetCurrentThreadId());
	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE myhandle;

	myhandle = (HANDLE)_beginthreadex(0, 0, &mythread;, 0, CREATE_SUSPENDED, 0);
	getchar();
	ResumeThread(myhandle);
	getchar();
	WaitForSingleObject(myhandle, INFINITE);
	CloseHandle(myhandle);

	return 0;
}
