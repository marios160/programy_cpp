#include <stdio.h>

#include "dispatcher.h"

int main(int argc, char ** argv) {
	if (argc > 1) {
		// Create list
		const unsigned int nodeCount = 5;
		Node_t * root = createList(nodeCount, 2, 4, 6, 8, 10);
		// Print created list
		printf("Created new list:\n");
		printList(root);
		// For all passed commands
		for (unsigned int i = 1; i < argc; ++i) {
			// Use predicate passed via argv list
			dispatch(&root, parseCommand(argv[i]));
			printf("After removal:\n");
			printList(root);
		}
	}
	return 0;
}