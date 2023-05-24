#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXC_LENZ 1024
#define MAX_ARGZ 64
#define MAXA_LENZ 128

char **read_stdins(char *input) {
    char **argz;
    char *sign;
    int x;

    argz = (char **) malloc(sizeof(char*) * MAX_ARGZ);
    if (argz == NULL) {
        return NULL;
    }

    sign = strtok(input, " ");
    for (x = 0; sign != NULL; x++) {
        if (x >= MAX_ARGZ - 1) {
            return NULL;
        }

        argz[x] = (char*) malloc(sizeof(char) * MAXA_LENZ);
        if (argz[x] == NULL) {
            return NULL;
        }

        strcpy(argz[x], sign);

        sign = strtok(NULL, " ");
    }

    argz[x] = NULL;

    return argz;
}

void enforce_cmd(char** arjs) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(arjs[0], arjs);
        printf("./shell: Command not found: %s\n", arjs[0]);
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        printf("Failed to fork process\n");
        exit(1);
    }
}

int main() {
    char *btn = NULL;
    size_t stdinz_size = 0;
    char **arjs;
    int i;

    while (1) {
        printf("$ ");
        if (getline(&btn, &stdinz_size, stdin) == -1) {
            printf("Error reading input\n");
            break;
        }

        if (btn[strlen(btn)-1] == '\n') {
            btn[strlen(btn)-1] = '\0';
        }

        arjs = read_stdins(btn);
        if (arjs == NULL) {
            printf("Failed to parse input\n");
            continue;
        }

        enforce_cmd(arjs);

        for (i = 0; arjs[i] != NULL; i++) {
            free(arjs[i]);
        }
        free(arjs);
    }

    free(btn);
    return 0;
}

