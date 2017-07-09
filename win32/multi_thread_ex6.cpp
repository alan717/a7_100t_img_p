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
	HANDLE myhandle[2];

	myhandle[0] = (HANDLE)_beginthreadex(0, 0, &mythread;, 0, 0, 0);
	myhandle[1] = (HANDLE)_beginthreadex(0, 0, &mythread;, 0, 0, 0);
	WaitForSingleObject(myhandle[0], INFINITE);
	WaitForSingleObject(myhandle[1], INFINITE);
	CloseHandle(myhandle[0]);
	CloseHandle(myhandle[1]);
	getchar();

	return 0;
}
