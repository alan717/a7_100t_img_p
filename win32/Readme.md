1. ##
HANDLE WINAPI CreateThread(
__in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes,
__in SIZE_T dwStackSize,
__in LPTHREAD_START_ROUTINE lpStartAddress,
__in_opt LPVOID lpParameter,
__in DWORD dwCreationFlags,
__out_opt LPDWORD lpThreadId
);


## CreateThread()函数


2. ## _beginthread()函数和_beginthreadex()函数 


uintptr_t _beginthread(
	void(*start_address )(void *),
	unsigned stack_size,
	void *arglist
);

uintptr_t _beginthreadex(
	void *security,
	unsigned stack_size,
	unsigned (*start_address) (void *),
	void *arglist,
	unsigned initflag,
	unsigned *thrdaddr
);

difference:

-1.两个函数不同在与第一个函数会自动退出，而第二个要调用CloseHandle()来显式的退出来释放资源。尽管线程自己退出

-2.第一个函数是void类型，调用默认是_cdecl方式，而第二个函数
返回的是unsigned int 类型，遵循_stdcall调用方式。

3. ##终止线程


ExitThread()函数和TerminiateThread()函数

_endthread()函数和_endthreadex()函数 


关闭其他线程需要使用closeHandle()

4. ## fork-join模型，
使用WaitForSingleObject()和WaitForMultipleObjects()函数 


5. ## resume suspended threads

唤醒挂起进程。
1).首先创建建成的时候让进程之后启动，之后使用ResumeThread()函数来启动进程。
2).还可以使用SuspendThread()进程来让已经在运行的进程来让它来挂起，
这个函数在调试器中场景中使用。
3)


