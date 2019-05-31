// Protector3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main()
{
    std::cout << "Hello World!\n"; 
	int len = 100;
	int v1 = len + 10;
	int v2 = v1 % 8;
	if (v2 != 0) v2 = 8 - v2;
	unsigned int v3 = v1 + v2;
	unsigned int v4 = v1;
	std::cout << "hello world!\n";
	v4 > v1 ? std::cout << "v4 great" : std::cout << "v1 great";
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
