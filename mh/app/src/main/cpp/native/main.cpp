//
// Created by think on 2018/10/9.
//
#include <sys/ptrace.h>
#include <stdio.h>
#include <android/log.h>
int main()
{

	if(ptrace(PTRACE_TRACEME,0,0,0)<0)
	{
		printf("gdb is debugging me ,exit.\n");
		return 1;

	}
	printf("No debugger ,continuing\n");


	return 0 ;
}
