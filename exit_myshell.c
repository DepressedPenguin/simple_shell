#include "myshell.h"
/**
 * exit_myshell- function that exits the program
 * @args: arguments
 * Return: None
 */
void exit_myshell(char **args)
{
if (strcmp(args[0], "exit") == 0)
{
if (args[1] != NULL)
{
int exit_status = atoi(args[1]);
exit(exit_status);
}
else
{
exit(0);
}
}
}
/**
 * change_directory - to Change the current working directory
 * @args: An array of arguments
 */
void change_directory(char **args)
{
char *new_directory;
char *previous_directory = current_directory;
if (args[1] == NULL || strcmp(args[1], "~") == 0)
{
new_directory = getenv("HOME");
}
else if (strcmp(args[1], "-") == 0)
{
new_directory = getenv("OLDPWD");
}
else
{
new_directory = args[1];
}
if (chdir(new_directory) == 0)
{
setenv("OLDPWD", current_directory, 1);
current_directory = getcwd(NULL, 0);
setenv("PWD", current_directory, 1);
}
else
{
printf("cd: %s: No such file or directory\n", new_directory);
}
if (previous_directory != NULL)
free(previous_directory);
}
/**
 * printing_env - Print all environment variables.
 */
void printing_env(void)
{
char **env = environ;
while (*env)
{
printf("%s\n", *env);
env++;
}
}
