#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

void tokenizeCommand(char* command, char*** args, int* argCount) {
    int argIndex = 0;
    char* token;

    token = strtok(command, " \t\n");
    while (token != NULL && argIndex < MAX_ARGS - 1) {
        (*args)[argIndex] = strdup(token);
        argIndex++;
        token = strtok(NULL, " \t\n");
    }
    (*args)[argIndex] = NULL;
    *argCount = argIndex;
}

void freeArguments(char** args) {
    int i;
    for (i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}

void executeCommand(char** args, const char* executableName, int commandNumber, char** envp) {
    char* command = args[0];
    char* path = getenv("PATH");
    char* token;

    while ((token = strtok(path, ":")) != NULL) {
        char executablePath[MAX_COMMAND_LENGTH];
        snprintf(executablePath, sizeof(executablePath), "%s/%s", token, command);

        if (access(executablePath, X_OK) == 0) {
            pid_t pid;
            int status;

            pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                if (execve(executablePath, args, envp) < 0) {
                    fprintf(stderr, "%s: %d: %s: not found\n", executableName, commandNumber, command);
                    exit(EXIT_FAILURE);
                }
            } else {
                do {
                    pid_t wpid = waitpid(pid, &status, WUNTRACED);
                    if (wpid == -1) {
                        perror("Waitpid failed");
                        exit(EXIT_FAILURE);
                    }
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }

            return;
        }

        path = NULL;
    }

    fprintf(stderr, "%s: %d: %s: not found\n", executableName, commandNumber, command);
}

int main(int argc, char *argv[], char *envp[]) {
    char command[MAX_COMMAND_LENGTH];
    char** args = NULL;
    int argCount = 0;
    int commandNumber = 0;

    FILE *inputStream = stdin;
    int interactiveMode = 1;

    if (argc > 1) {
        inputStream = fopen(argv[1], "r");
        interactiveMode = 0;
        if (inputStream == NULL) {
            perror("Failed to open the script file");
            exit(EXIT_FAILURE);
        }
    }

    while (1) {
        if (isatty(0)) {
            printf("$ ");
            fflush(stdout);
        }

        if (fgets(command, sizeof(command), inputStream) == NULL) {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            break;
        }

        commandNumber++;

        args = (char**) malloc((MAX_ARGS + 1) * sizeof(char*));
        if (args == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        tokenizeCommand(command, &args, &argCount);
        executeCommand(args, argv[0], commandNumber, envp);
        freeArguments(args);
    }

    if (!interactiveMode) {
        fclose(inputStream);
    }

    return 0;
}

