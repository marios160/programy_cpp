#include <stdio.h>

#include "list.h"
#include "comparators.h"

int main() {
	// Create list
	const unsigned int nodeCount = 5;
	Node_t * root = createList(nodeCount, 2, 4, 6, 8, 10);
	// Print created list
	printf("Created new list:\n");
	printList(root);
	// Remove first element
	printf("Removed first element:\n");
	removeIf(&root, isEqual, 2);
	printList(root);
	// Remove last element
	printf("Removed last element:\n");
	removeIf(&root, isGreater, 9);
	printList(root);
	// Remove first element less than 8
	printf("Removed first element less than 8:\n");
	removeIf(&root, isLess, 8);
	printList(root);
	return 0;
}