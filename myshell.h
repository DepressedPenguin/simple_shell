#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <ctype.h>
#define MAXC_LEN 1024
#define MAX_ARZ 64
#define MAXA_LEN 128
#define BUFFER_SIZE 1024

extern char **environ;
char *current_directory;
char **exe_name_cmd;
/**
 * read_stdin - Reads input from standard into an array of strings.
 * @input: The input string to be.
 *
 * Return: The array of strings.
 */
char **read_stdin(char *input);

/**
 * find_wayy - Finds the full path .
 * @commands: The command to search forin the full path.
 *
 * Return: always return to The full path of the command.
 */
char *find_wayy(char *commands);

/**
 * exes_cmds - Executes the given commands.
 * @args: The array to execute.
 */
void exes_cmds(char **args);

/**
 * exit_myshell - Handles the exit command.
 * @args: The array of arguments.
 */
void exit_myshell(char **args);

/**
 * change_directory - Handles the cd command.
 * @args: The array of arguments .
 */
void change_directory(char **args);

/**
 * printing_env - Prints the environment variables.
 */
void printing_env(void);

/**
 * _getly - Custom implementation of the getline function.
 * @lineptr: Pointer to the buffer .
 * @n: Pointer to the size .
 * @stream: The input stream .
 *
 * Return: The number of characters read.
 */
ssize_t _getly(char **lineptr, size_t *n, FILE *stream);

#endif /* _SHELL_H_ */

