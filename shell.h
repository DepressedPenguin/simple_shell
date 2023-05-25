#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/* Function Declarations */
void tokenizeCommand(char *command, char ***args, int *argCount);
void freeArguments(char **args);
void executeCommand(char **args, const char *executableName,
	int commandNumber);
void processInput(FILE *inputStream, char *programName);

#endif /* SHELL_H */

