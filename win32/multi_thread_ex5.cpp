#include <Windows.h>
#include <process.h>

unsigned int __stdcall mythreadA(void* data) 
{
	return 0;
}

unsigned int __stdcall  mythreadB(void* data)
{
	volatile int i;

	// Most compiler won't eliminate the loop
	// since i is volatile
	for (i = 0; i < 100000; i++) {}

	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE myhandleA, myhandleB;

	myhandleA = (HANDLE)_beginthreadex(0, 0, &mythreadA;, 0, 0, 0);
	myhandleB = (HANDLE)_beginthreadex(0, 0, &mythreadB;, 0, 0, 0);
	WaitForSingleObject(myhandleA, INFINITE);
	WaitForSingleObject(myhandleB, INFINITE);
	CloseHandle(myhandleA);
	CloseHandle(myhandleB);

	return 0;
}
