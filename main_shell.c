#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* tokenizeCommand - Tokenize the command into arguments
* @command: The command string
* @args: Pointer to the argument array
* @argCount: Pointer to the argument count variable
*/
void tokenizeCommand(char *command, char ***args, int *argCount)
{
char *token;
int argIndex = 0;

token = strtok(command, " \t\n");
while (token != NULL && argIndex < MAX_ARGS - 1)
{
(*args)[argIndex] = strdup(token);
argIndex++;
token = strtok(NULL, " \t\n");
}
(*args)[argIndex] = NULL;
*argCount = argIndex;
}

/**
* freeArguments - Free the memory allocated for the argument array
* @args: The argument array
*/
void freeArguments(char **args)
{
int i;
for (i = 0; args[i] != NULL; i++)
{
free(args[i]);
}
free(args);
}

/**
* executeCommand - Execute the command in a child process
* @args: The argument array
* @executableName: The name of the executable
* @commandNumber: The number of the command
*/
void executeCommand(char **args, const char *executableName, int commandNumber)
{
pid_t pid;
int status;

pid = fork();
if (pid < 0)
{
perror("Fork failed");
exit(EXIT_FAILURE);
	}
else if (pid == 0)
	{
if (execvp(args[0], args) < 0)
	{
fprintf(stderr, "%s: %d: %s: not found\n",
executableName, commandNumber, args[0]);
exit(EXIT_FAILURE);
}
}
else
{
do	{
pid_t wpid = waitpid(pid, &status, WUNTRACED);
if (wpid == -1)
	{
	perror("Waitpid failed");
	exit(EXIT_FAILURE);
	}
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	}

