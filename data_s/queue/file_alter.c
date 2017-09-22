#include <stdio.h>
#include <stdlib.h>

int main()
{
	long position=0;
	FILE  *pf=fopen("./save2.dat","rw");
	while(!feof(pf)){
	int x;
	fscanf(pf,"%d",&x);
	if(x==68)
	{
		position=ftell(pf);

		break;
	}
	}
	fseek(pf,position,SEEK_SET);
	fwrite("100",1,3,pf);
	fclose(pf);


	return 0;
}
