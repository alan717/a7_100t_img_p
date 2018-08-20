#include <iostream>

int main()
{
	// A character array in C/C++/Java
	char arr1[] = { 'g', 'e', 'e', 'k', 's' };

	// An Integer array in C/C++/Java
	int arr2[] = { 10, 20, 30, 40, 50 };

	// Item at i'th index in array is typically accessed
	// as "arr[i]".  For example arr1[0] gives us 'g'
	// and arr2[3] gives us 40.
	std::cout << "hello world " << std::endl;
	char* pPath;
	pPath = getenv("PATH");
	if (pPath != NULL)
		printf("The current path is: %s", pPath);
	return 0;
}	