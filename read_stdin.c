#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXC_LEN 1024
#define MAX_ARZ 64
#define MAXA_LEN 128

char **exe_name_cmd;

char **read_stdin(char *input) {
    char **argz = malloc(sizeof(char*) * MAX_ARZ);
    char *toksign;
    int v = 0;

    if (argz == 0) {
        return (NULL);
    }

    toksign = strtok(input, " ");
    
    for ( v = 0 ; toksign != 0 ; v++) {
        if (v >= MAX_ARZ - 1) {
            return (NULL);
        }

        argz[v] = malloc(sizeof(char) * MAXA_LEN);
        if (argz[v] == 0) {
             return (NULL);
        }

        strcpy(argz[v], toksign);

        toksign = strtok(NULL, " ");
    }

    argz[v] = NULL;

    return argz;
}
