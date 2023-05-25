#include "yzshell.h"
/**
 * fun_bult - Check if the command is a built-in
 * @args: The array of command arguments.
 * Return: 1 if the command is a built-in or 0
 */
int fun_bult(char **args)
{
return (strcmp(args[0], "exit") == 0 ||
strcmp(args[0], "cd") == 0 ||
strcmp(args[0], "env") == 0);
}
/**
 * pars_args - Parse the input line into arguments.
 * @linP: The input line.
 * Return: An array of strings representing.
 */
char **pars_args(char *linP)
{
char **args = malloc(IMAX_ARGS * sizeof(char *));
char *tkn;
int i = 0;
tkn = strtok(linP, " \t\n");
while (tkn != NULL && i < IMAX_ARGS)
{
args[i++] = strdup(tkn);
tkn = strtok(NULL, " \t\n");
}
args[i] = NULL;
return (args);
}
/**
 * print_out - Print the shell prompt.
 */
void print_out(void)
{
write(STDOUT_FILENO, "#yassine$ ", 9);
}
/**
 * print_myid - Print the process ID (PID).
 */
void print_myid(void)
{
char pid_str[100];
int pid_len;
pid_len = sprintf(pid_str, "PID: %d\n", getpid());
write(STDOUT_FILENO, pid_str, pid_len);
}
/**
 * print_cid - Print the parent process ID (PPID).
 */
void print_cid(void)
{
char ppid_str[100];
int ppid_len;
ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
write(STDOUT_FILENO, ppid_str, ppid_len);
}
