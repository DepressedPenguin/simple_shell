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

#define MAXC_LENZ 1024
#define MAX_ARGZ 64
#define MAXA_LENZ 128
char **read_stdins(char *input);
void enforce_cmd(char** args);
#endif
