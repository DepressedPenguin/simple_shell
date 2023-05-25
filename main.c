#include "shell.h"

/**
 * main - Entry point of the shell program
 * @argc: The number of command-line arguments
 * @argv: An array of command-line argument strings
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
FILE *inputStream = stdin;
int interactiveMode = 0;

    /* Check if a script file is provided */
if (argc > 1)
{
inputStream = fopen(argv[1], "r");
interactiveMode = 0;
if (inputStream == NULL)
{
perror("Failed to open the script file");
exit(EXIT_FAILURE);
}
}

    /* Process user input */
processInput(inputStream, argv[0]);

    /* Close the input stream if not in interactive mode */
if (!interactiveMode)
fclose(inputStream);

return (0);
}
