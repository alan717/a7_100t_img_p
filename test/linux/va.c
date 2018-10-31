#include "stdio.h"
#include "stdarg.h"

int aveint(int,...);


int main()
{
	printf("%d\t",aveint(2,2,3));
	printf("%d\t",aveint(5,2,4,3,7,8));
	return 0;




}


int aveint(int v,...)
{
	int ReturnValue=0;

	int i=v;
	va_list ap;
	va_start(ap,v);
	while(i>0)
	{
		ReturnValue+=va_arg(ap,int);
		i--;
	}

	va_end(ap);
	return ReturnValue/=v;
}









