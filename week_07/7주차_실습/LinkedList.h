#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__
#define LEN 100

typedef struct node {

	int score;
	char name[LEN];
	struct node* next;
	struct node* prev;
} Node;

typedef struct linkedList {

	Node* first;
	int count;
} LinkedList;

void ListInit(LinkedList* list);
void ListInsert(LinkedList* list, int newScore, char* newName);

void printList(LinkedList* list);

void printListRange(LinkedList* list, int start, int end);

#endif
