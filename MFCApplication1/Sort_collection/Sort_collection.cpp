// Sort_collection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
//void Bubble_sort(int array[],int len);
//void Merge_sort (int);


#define DEF_FUNC( ret,func_name,...)  void func_name(__VA_ARGS__);

DEF_FUNC(void, swap, int*, int*)
DEF_FUNC(void, print      , int array[], int len)
DEF_FUNC(void, genRandNum,int array[],int len,int maxl)// gen rand num array
//DEF_FUNC(void, MergeSort , int array[],int len)//1
//DEF_FUNC(void, SelectSort, int array[],int len)//2
DEF_FUNC(void, bubleSort , int array[],int len)//3
//DEF_FUNC(void, InsertSort, int array[],int len)//4
//DEF_FUNC(void, QuickSort , int array[],int len)//5
//DEF_FUNC(void, QuickSort2, int array[],int len)// optimization edition
//DEF_FUNC(void, HeapSort  , int array[],int len)//6
//DEF_FUNC(void, ShellSort , int array[],int len)//7
//DEF_FUNC(void, CountSort , int array[],int len)//8
//DEF_FUNC(void, BucketSort, int array[],int len)//9
//DEF_FUNC(void, RadixSort , int array[],int len)//10

int main()
{
	int array[] = {9,8,8,7,6,5,4,3,2,2,1};
	int len = sizeof(array) / sizeof(array[0]);
	//Merge_sort(array, len);
	//swap(&array[0],&array[1]);
	
	bubleSort(array,len);
	genRandNum(array, len,10);
	
	
    return 0;
}

void print(int array[], int len)
{
	for (int i = 0; i < len ; i++)
	{
		if (i == 0) { 
			printf("array: %d,",array[i]); 
		}
		else if (i == len - 1) {
			printf("%d",array[i]);
		}else
		{
			printf("%d,", array[i]);
		}
	}
	printf("\n");
}
void swap(int* a,int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void bubleSort(int array[],int len)
{
	print(array, len);
	for(int i=0;i<len;i++)
		for (int j = 0; j < len - 1 - i; j++)
		{
			if (array[j] > array[j+1])
				swap(&array[j],&array[j+1]);
		}
	print(array, len);
}

void Merge_sort(int array[], int len)
{


}
void genRandNum(int array[], int len,int maxValue) {
	std::srand(std::time(nullptr));
	for (int n = 0; n != len; ++n) {
		int x = maxValue + 1;
		while (x > maxValue)
			x = 1 + std::rand() / ((RAND_MAX + 1u) / maxValue);  // Note: 1+rand()%maxValue is biased
		//std::cout << x << ' ';
		array[n] = x;
	}
}