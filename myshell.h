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
#define MAXC_LEN 1024
#define MAX_ARZ 64
#define MAXA_LEN 128
#define BUFFER_SIZE 1024
extern char **environ;
char **exe_name_cmd;
char *current_directory;
char **read_stdin(char *input);
char* find_wayy(char* commands);
void exes_cmds(char** args);
void exit_myshell(char** args);
void change_directory(char **args);
void printing_env(void);
ssize_t _getly(char **lineptr, size_t *n, FILE *stream);

#endif

