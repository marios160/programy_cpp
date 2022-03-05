#pragma once

#define MAX_INSTRUCTION_LENGTH 12

typedef struct ParsedCommand {
	char instruction[MAX_INSTRUCTION_LENGTH];
	int argument;
} ParsedCommand_t;

ParsedCommand_t parseCommand(const char * command);
