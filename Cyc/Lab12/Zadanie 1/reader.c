#include <stdio.h>

#include "reader.h"

char readChar() {
	char character;
	while ((character = getchar()) == '\n' || character == EOF) {}
	return character;
}
