#include "stdafx.h"
#include "jd.h"
#include <cstdlib>

#include<cstring>
#include<iostream>
#include<ctime>

#define m 0x100000000LL
#define c 0xB16
#define a 0x5DEECE66DLL

static unsigned long long seed = 1;
void srand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}

int __fastcall GenrandomKey(int a1)
{
	int v1; // r9
	time_t  v2; // r0
	unsigned int v3; // r0
	char *v4; // r4
	int v6; // [sp+0h] [bp-78h]
	int v7; // [sp+4h] [bp-74h]
	int v8; // [sp+8h] [bp-70h]
	int v9; // [sp+Ch] [bp-6Ch]
	char v10; // [sp+10h] [bp-68h]
	char v11[64]; // [sp+14h] [bp-64h]

	v1 = a1;
	strcpy_s(v11,64, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	v6 = 0;
	v7 = 0;
	v8 = 0;
	v9 = 0;
	v10 = 0;
	v2 = time(0);
	//v3 = rand48(v2);
	v4 = (char *)&v6;
	do
	{
		v3 =rand()%62;
		*v4++ = v11[v3];
	} while (v4 != &v10);
	return 1;
}