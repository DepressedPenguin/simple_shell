#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_LEN 1024
#define MAX_ARGS 64
#define MAX_ARG_LEN 128

char **read_stdins(char *input) {
    char **args;
    char *token;
    int argc = 0;

    args = (char **) malloc(sizeof(char*) * MAX_ARGS);
    if (args == NULL) {
        perror("malloc");
        return NULL;
    }

    token = strtok(input, " ");
    while (token != NULL) {
        if (argc >= MAX_ARGS - 1) {
            fprintf(stderr, "Too many arguments\n");
            return NULL;
        }

        args[argc] = strdup(token);
        if (args[argc] == NULL) {
            perror("strdup");
            return NULL;
        }

        token = strtok(NULL, " ");
        argc++;
    }

    args[argc] = NULL;

    return args;
}

void enforce_cmd(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
}

int main() {
    char *input = NULL;
    size_t input_size = 0;
    char **args;
    int i;

    while (1) {
        printf("$ ");
        if (getline(&input, &input_size, stdin) == -1) {
            perror("getline");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        args = read_stdins(input);
        if (args == NULL) {
            fprintf(stderr, "Invalid input\n");
            continue;
        }

        enforce_cmd(args);

        for (i = 0; args[i] != NULL; i++) {
            free(args[i]);
        }
        free(args);
    }

    free(input);
    return 0;
}

