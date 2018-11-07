#include<iostream>
#include"CList.h"
#include"CStack.h"
#include"CQueue.h"

using namespace std;

int main()
{
	CQueue<int>* cQueue = new CQueue<int>();
	CStack<double>* cStack = new CStack<double>();
	for (int i = 0; i < 100; i++)
	{
		cQueue->Enqueue(new int(i));
		cStack->Push(new double(rand() / 10.0));
	}
	cout << "Queue - integer collection:" << endl;
	for (; cQueue->Count();)
	{
		cout << *cQueue->Dequeue() << " ";
	}
	cout << endl << endl << "Stack - double collection:" << endl;
	for (; cStack->Count();)
	{
		cout << *cStack->Pop() << " ";
	}
	delete cQueue;
	delete cStack;
	cout << endl << endl;
	return system("pause");
}