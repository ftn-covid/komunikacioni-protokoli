#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct node {
	int value;
	struct node* next;
} node_t;

void printList(node_t* head);
void addToListBeginning(node_t** head, int value);
void addToEndList(node_t* head, int value);
void freeList(node_t* head);
int searchList(node_t* head, int value);

int main()
{
	node_t* head = (node_t*)malloc(sizeof(node_t));
	head->value = 0;
	head->next = NULL;

	int i = 0;
	clock_t start_time = clock();
	for (i = 0; i < 10000; i++) addToListBeginning(&head, rand() % (i + 1));
	clock_t end_time = clock();

	printf("Time passed by when trying to add to the start of the list: %d ms.\n\n", end_time - start_time);
	freeList(head);

	head = (node_t*)malloc(sizeof(node_t));
	head->value = 0;
	head->next = NULL;

	start_time = clock();
	for (i = 0; i < 10000; i++) addToEndList(head, rand() % (i + 1));
	end_time = clock();

	printf("Time passed by when trying to add to the end of the list: %d ms.\n\n", end_time - start_time);

	start_time = clock();
	for (int i = 0; i < 10000; i++) {
		int num = rand() % (i + 1);
		if (searchList(head, num) == -1) printf("Num %d was not found in the list\n", num);
	}
	end_time = clock();

	printf("Time passed by when trying to search elements of the list: %d ms.\n\n", end_time - start_time);
	return 0;
}
void freeList(node_t* head) {
	while (head != NULL) {
		node_t* temp = head;
		head = head->next;
		free(temp);
	}
}
void printList(node_t* head) {
	node_t* current = head;

	while (current != NULL) {
		printf("Value: %d\n", current->value);
		current = current->next;
	}
}

void addToListBeginning(node_t** head, int value) {
	node_t* new = (node_t*)malloc(sizeof(node_t));

	new->next = *head; //original head pointer
	*head = new; //switch the two pointers
	new->value = value; //set the value of the new node
}

void addToEndList(node_t* head, int value) {
	node_t* current = head;
	while (current->next != NULL) {
		current = current->next; //loop through the list until we get to the end
	}

	current->next = (node_t*)malloc(sizeof(node_t));
	current->next->value = value;
	current->next->next = NULL;
}

int searchList(node_t* head, int value) {
	node_t* current = head;
	while (current != NULL) {
		if (current->value == value)
			return 1;
		current = current->next;
	}
	return -1;
}