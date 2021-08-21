#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

void ListInit(LinkedList* list) {

	list->first = NULL;
	list->count = 0;
}

void ListInsert(LinkedList* list, int newScore, char* newName) {

	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->score = newScore;
	strcpy(newNode->name, newName);
	newNode->prev = newNode->next = NULL;

	if (list->count == 0) 
		list->first = newNode;
	else if (newScore > list->first->score) {
		newNode->next = list->first;
		list->first->prev = newNode;
		list->first = newNode;
	}
	else {

		Node* temp = list->first;
			
		while (1) {

			if (temp->next == NULL) {

				temp->next = newNode;
				newNode->prev = temp;
				break;
			}
			if (temp->score >= newScore && temp->next->score <= newScore) {

				temp->next->prev = newNode;
				newNode->next = temp->next;
				temp->next = newNode;
				newNode->prev = temp;
				break;
			}
			temp = temp->next;
		}	
	}
	(list->count)++;
}
		
void printList(LinkedList* list) {

	int i, n;

	if (list->count == 0) {
		printw("Rank Empty!! \n");
		return;
	}

	Node* temp = list->first;

	for (i = 0; i < list->count; i++, temp = temp->next)
		printw(" %-16s|   %d \n", temp->name, temp->score);

}

void printListRange(LinkedList* list, int start, int end) {

	int i;
    
	if (list->count == 0) {
		printw("Rank Empty!! \n");
		return;
	}

	Node* temp = list->first;

	for (i = 1; i <= list->count; i++, temp = temp->next)
		if (i >= start && i <= end)
			printw(" %-16s|   %d \n", temp->name, temp->score);
	
}
