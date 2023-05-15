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
#define MAXC_LEN 1024
#define MAX_ARZ 64
#define MAXA_LEN 128
#include <stddef.h>
#define BUFFER_SIZE 1024
char **exe_name_cmd;
char **read_stdins(char* input);
void enforce_cmd(char** args);

char* _mygitl(void);
void enforce_cmd(char** arjs);
void exes_cmds(char** args);
void exit_myshell(char** args);
void printing_env(void);
char* find_wayy(char* commands);
#endif

