/************************************************
*
* File Name : queue_t.cpp
* Purpose :
* Creation Date : 16-09-2017
* Last Modified : Sat 16 Sep 2017 07:27:11 PM HKT
* Created By :
* Description :
*
************************************************/
#include <stdlib.h>
#include <stdio.h>
struct Node
{
	int key;
	struct Node *next;
};

struct Queue
{
	struct Node *front,*rear;
};


struct Node* newNode(int k)
{
	struct Node *temp=(struct Node*)malloc(sizeof(struct Node));
	temp->key=k;
	temp->next=NULL;
	return temp;
}


struct Queue *createQueue()
{
	struct Queue *q=(struct Queue*)malloc(sizeof(struct Queue));
	q->front=q->rear=NULL;
	return q;
}

void enQueue(struct Queue *q,int k)
{


}

