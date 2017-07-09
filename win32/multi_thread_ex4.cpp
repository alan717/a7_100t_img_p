#include <Windows.h>
#include <process.h>
#include <stdio.h>

void mythreadA(void* data)
{
	printf("mythreadA %d \n", GetCurrentThreadId());
}

void mythreadB(void* data)
{
	volatile int i;

	// Most compiler won't eliminate the loop
	// since i is volatile
	for (i = 0; i < 100000; i++) {}

	printf("mythreadB %d \n", GetCurrentThreadId());
}

int main(int argc, char* argv[])
{
	HANDLE myhandleA, myhandleB;

	myhandleA = (HANDLE)_beginthread(&mythreadA;, 0, 0);
	myhandleB = (HANDLE)_beginthread(&mythreadB;, 0, 0);
	WaitForSingleObject(myhandleA, INFINITE);
	WaitForSingleObject(myhandleB, INFINITE);

	return 0;
}
