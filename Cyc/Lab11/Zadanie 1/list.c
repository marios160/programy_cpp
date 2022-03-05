#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static Node_t * createNode(int head) {
	Node_t * node = (Node_t *) malloc(sizeof(node));
	node->head = head;
	node->tail = NULL;
	return node;
}

void push(Node_t * root, int value) {
	Node_t * currentNode = root;
	if (currentNode != NULL) {
		while (currentNode->tail != NULL)
			currentNode = currentNode->tail;
		currentNode->tail = createNode(value);
	}
}

void printList(Node_t * root) {
	Node_t * currentNode = root;
	while (currentNode != NULL) {
		printf("%d\n", currentNode->head);
		currentNode = currentNode->tail;
	}
}

Node_t * createList(unsigned int nodeCount, ...){
    va_list args;
    va_start(args , nodeCount);
    Node_t * root;
    for(int i = 0; i < nodeCount; i++){
        int value = va_arg(args , int);
        if(i == 0){
            root = createNode(value);
        } else {
            push(root,value);
        }
    }
    va_end(args);
    return root;
}
