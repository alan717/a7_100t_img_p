/************************************************
*
* File Name : queue_t.cpp
* Purpose :
* Creation Date : 16-09-2017
* Last Modified : Fri 22 Sep 2017 11:58:09 PM HKT
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
	struct Node *temp=newNode(k);

	if(q->rear==NULL)
	{
		q->front=q->rear=temp;
		return ;
	}
	
	q->rear->next=temp;
	q->rear=temp;
}

struct Node *deQueue(struct Queue *q)
{
	if(q->front==NULL)
		return NULL;
	
	struct Node *temp=q->front;
	q->front=q->front->next;
	if(q->front==NULL)
		q->rear=NULL;
	return temp;

}

int main()
{
	struct Queue *q=createQueue();
	enQueue(q,10);
	enQueue(q,20);
	deQueue(q);
	deQueue(q);
	enQueue(q,30);
	enQueue(q,40);
	enQueue(q,50);
	struct Node *n=deQueue(q);
	while(n!=NULL)
	{
		printf("item is %d\n",n->key);
		n=deQueue(q);
	}
	return 0;
}


