// Sort_collection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//void Bubble_sort(int array[],int len);
//void Merge_sort (int);


#define desort_func( func_name)  void func_name(int array[],int len)

desort_func(Merge_sort);


int main()
{
	int array[] = {9,8,8,7,6,5,4,3,2,2,1};
	int len = sizeof(array) / sizeof(array[0]);
	Merge_sort(array, len);


    return 0;
}

