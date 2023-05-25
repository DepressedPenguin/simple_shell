#include "shell.h"

/**
 * processInput - Process the input commands from a file or stdin
 * @inputStream: The input stream to read the commands from
 * @programName: The name of the program
 */
void processInput(FILE *inputStream, char *programName)
{
char **args = NULL, command[MAX_COMMAND_LENGTH];
int argCount = 0, commandNumber = 0;
while (1)
{
if (isatty(0))
{
printf("$ ");
fflush(stdout);
}
if (fgets(command, sizeof(command), inputStream) == NULL)
{
printf("\n");
break;
}
command[strcspn(command, "\n")] = '\0';
if (strcmp(command, "exit") == 0)
break;
commandNumber++;
args = (char **)malloc((MAX_ARGS + 1) * sizeof(char *));
if (args == NULL)
{
perror("Memory allocation failed");
exit(EXIT_FAILURE);
}

tokenizeCommand(command, &args, &argCount);
executeCommand(args, programName, commandNumber);
freeArguments(args);
}
}
