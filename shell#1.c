#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
char *command = NULL;
size_t bufsize = 0;

while (1)
{
printf("#cisfun$ ");
if (getline(&command, &bufsize, stdin) == -1)
{
if (feof(stdin))
{
printf("\n");
break;
}
perror("getline");
exit(EXIT_FAILURE);
}
if (command[strlen(command) - 1] == '\n')
command[strlen(command) - 1] = '\0';

if (access(command, X_OK) == 0) 
{
if (fork() == 0) 
{     
char **argv = malloc(2 * sizeof(char *));
argv[0] = command;
argv[1] = NULL;

if (execve(command, argv, environ) == -1)
{
perror(command);
exit(EXIT_FAILURE);
}
break;
}
else
{
wait(NULL);
}
} 
else
{
fprintf(stderr, "%s: No such file or directory\n", command);
}
}

free(command);
return EXIT_SUCCESS;
}
