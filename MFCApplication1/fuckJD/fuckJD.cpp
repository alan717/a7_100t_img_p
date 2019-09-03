// fuckJD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
 
#include <iostream>

#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif
int   sub_13720( int a1,  int a2)
{
	int v2; // r12
	unsigned int v3; // r3
	int v4; // r2
	int result; // r0

	v2 = a1 ^ a2;
	v3 = 1;
	v4 = 0;
	if ((a2 & 0x80000000) != 0)
		a2 = -a2;
	if ((a1 & 0x80000000) != 0)
		a1 = -a1;
	if (a1 >= a2)
	{
		while (a2 < 0x10000000 && a2 < a1)
		{
			a2 *= 16;
			v3 *= 16;
		}
		while (a2 < 0x80000000 && a2 < a1)
		{
			a2 *= 2;
			v3 *= 2;
		}
		while (1)
		{
			if (a1 >= a2)
			{
				a1 -= a2;
				v4 |= v3;
			}
			if (a1 >= a2 >> 1)
			{
				a1 -= a2 >> 1;
				v4 |= v3 >> 1;
			}
			if (a1 >= a2 >> 2)
			{
				a1 -= a2 >> 2;
				v4 |= v3 >> 2;
			}
			if (a1 >= a2 >> 3)
			{
				a1 -= a2 >> 3;
				v4 |= v3 >> 3;
			}
			if (!a1)
				break;
			v3 >>= 4;
			if (!v3)
				break;
			a2 >>= 4;
		}
	}
	result = v4;
	if (v2 < 0)
		result = -v4;
	return result;
}
int main()
{
	unsigned int sec = 0x5d4b96da; // sec from time_val struct  data;
	unsigned int div = 0x3e8;  //1000 divider.
	unsigned int lows = sec << 16;
	lows >>= 16;
	unsigned int high = sec >> 16;

 	unsigned int ret = lows * div;
	unsigned int ret2 = high * div;
	unsigned int lows_ret = ret << 16;
	lows_ret >>= 16;

	printf("calc ret:%x,%x\n", ret,ret2);
	//(ret2 + 0)*high;
	ret >>= 16;
	unsigned int  r7=ret << 16;
	unsigned int sum =ret2 + ret;
	printf("calc ret2:%x,%x,%x\n", ret, ret2, sum);
	unsigned int high_sum=sum >> 16;
	unsigned int lows_sum = sum << 16; //lows_sum >>= 16;

	unsigned int calu=lows_sum + lows_ret;

	printf("check %x,calcu:%x,ret:%x\n",sum,calu,ret2>>16);
	unsigned int r1 = 0x16C;
	
	//unsigned int lWord_sec = sec << 16;
	//lWord_sec >>= 16;
	//unsigned int hight = sec >> 16;
	//unsigned int ret = lWord_sec * 0x3e8;
	//unsigned int ret2 = hight * 0x3e8;
	//unsigned int lows_ret = ret << 16;
	//lows_ret >>= 16;
	//ret >>= 16;
	//unsigned int sum = ret2 + ret;
	//unsigned int lows_sum = sum << 16;
	//unsigned int calu = lows_sum + lows_ret;
	//printf("ssss:%x,\n",calu);
	int sss=sub_13720(0xd14a8, 1000);
	//sss + calu;
	printf("simu:%x , %x\n\n", sss, sss + calu);
	
	
	////// new start =========================
	unsigned int tv_sec = 0x5D4C116D;
	unsigned int tv_usec = 0x0006A0AF;
	unsigned int res= 1000 * tv_sec + tv_usec / 1000;
	printf("check :%x\n",res);
	//if (res) {
	//	do {
	//		=(res%10)+48
	//	}while(res)
	//}
	unsigned int aaaa =( ret2+res) % 0xA + 48;
	printf("check :%x\n", aaaa);
    //std::cout << "Hello World!\n"; 
	
	timeval tv;
	gettimeofday(&tv, nullptr);
	printf("check :0x%x£¬0x%x\n", tv.tv_sec,tv.tv_usec);
	return 0;
}


 
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
