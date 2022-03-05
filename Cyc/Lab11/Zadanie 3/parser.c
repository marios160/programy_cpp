#include <stdlib.h>
#include <string.h>

#include "parser.h"

ParsedCommand_t parseCommand(const char * command) {
    ParsedCommand_t parsedCommand;
	unsigned int commandLength = strlen(command) + 1;
    char buffer[commandLength];
    strncpy(buffer, command, commandLength);
    const char * delimiter = " ";
    char * part = strtok(buffer, delimiter);
    if (part != NULL) {
        strncpy(parsedCommand.instruction, part, sizeof(parsedCommand.instruction));
		part = strtok(NULL, delimiter);
        parsedCommand.argument = atoi(part);
    }
    return parsedCommand;
}
