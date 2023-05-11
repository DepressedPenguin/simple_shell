#include "myshell.h"
#include <string.h>
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

char **read_stdins(char *input) {
    char **argz = (char **) malloc(sizeof(char*) * MAX_ARGZ);
    char *sign;
    int x = 0;
    if (argz == 0) {
        return (NULL);
    }

    sign = strtok(input, " ");
    for (x = 0 ; sign != NULL ;x++) {
        if (x >= MAX_ARGZ - 1) {
        return (NULL);
        }

        argz[x] = (char*) malloc(sizeof(char) * MAXA_LENZ);
        if (argz[x] == 0) {
         return (NULL);
        }

        strcpy(argz[x], sign);

        sign = strtok(NULL, " ");
    }

    argz[x] = NULL;

    return (argz);
}
