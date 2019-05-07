// Sort_collection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//void Bubble_sort(int array[],int len);
//void Merge_sort (int);


#define DEF_FUNC( ret,func_name,...)  void func_name(__VA_ARGS__);

DEF_FUNC(void, swap, int*, int*)
DEF_FUNC(void,Merge_sort, int array[], int len)
DEF_FUNC(void, bubleSort, int array[], int len)
DEF_FUNC(void,print,int array[],int len)

int main()
{
	int array[] = {9,8,8,7,6,5,4,3,2,2,1};
	int len = sizeof(array) / sizeof(array[0]);
	//Merge_sort(array, len);
	//swap(&array[0],&array[1]);
	bubleSort(array,len);
	print(array,len);
	
    return 0;
}

void print(int array[], int len)
{
	for (int i = 0; i < len - 1; i++)
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
}
void swap(int* a,int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void bubleSort(int array[],int len)
{
	for(int i=0;i<len;i++)
		for (int j = 0; j < len - 1 - i; j++)
		{
			if (array[j] > array[j+1])
				swap(&array[j],&array[j+1]);
		}

}

void Merge_sort(int array[], int len)
{

}

