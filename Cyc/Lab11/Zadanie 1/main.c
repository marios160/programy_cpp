#include <stdio.h>

#include "list.c"

int main() {
	// Create list
	const unsigned int nodeCount = 5;
	Node_t * root = createList(nodeCount, 2, 4, 6, 8, 10);
	// Print created list
	printf("Created new list:\n");
	printList(root);
	return 0;
}
