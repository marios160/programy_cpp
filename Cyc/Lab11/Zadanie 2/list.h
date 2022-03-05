#pragma once

#include "predicate.h"

typedef struct Node {
	int head;
	struct Node * tail;
} Node_t;

Node_t * createList(unsigned int nodeCount, ...);
void printList(Node_t * root);
void push(Node_t * root, int value);
void removeIf(Node_t ** root, Predicate predicate, int toCompare);
