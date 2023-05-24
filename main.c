#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
extern char **environ;

void tokenizeCommand(char* command, char*** args, int* argCount) {
    char* token;
    int argIndex = 0;
    
    token = strtok(command, " \t\n");
    if (token == NULL) {
        *argCount = 0;
        return;
    }
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

void executeCommand(char** args, const char* executableName, int commandNumber) {
    pid_t pid;
    int status;

    // Check if the command exists in the PATH
    char* path = getenv("PATH");
    char* pathEntry;
    char commandPath[MAX_COMMAND_LENGTH];
    int commandFound = 0;

    while ((pathEntry = strsep(&path, ":")) != NULL) {
        snprintf(commandPath, sizeof(commandPath), "%s/%s", pathEntry, args[0]);
        if (access(commandPath, X_OK) == 0) {
            commandFound = 1;
            break;
        }
    }

    if (!commandFound) {
        fprintf(stderr, "%s: %d: %s: command not found\n", executableName, commandNumber, args[0]);
        return;
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execve(commandPath, args, environ) == -1) {
            perror("Execve failed");
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
}

int changeDirectory(char** args, int argCount) {
    if (argCount != 2) {
        fprintf(stderr, "cd: invalid number of arguments\n");
        return 1;
    }

    if (chdir(args[1]) != 0) {
        perror("cd failed");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char* command = NULL;
    char** args = NULL;
    int argCount = 0;
    int commandNumber = 0;
    ssize_t lineSize;
    size_t bufferSize = 0;
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

        bufferSize = 0;
        lineSize = getline(&command, &bufferSize, inputStream);
        if (lineSize == -1) {
            printf("\n");
            break;
        }
        command[lineSize - 1] = '\0';

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
        if (argCount > 0) {
            if (strcmp(args[0], "cd") == 0) {
                if (changeDirectory(args, argCount) != 0) {
                    fprintf(stderr, "Failed to change directory\n");
                }
            } else {
                executeCommand(args, argv[0], commandNumber);
            }
        }

        freeArguments(args);
    }

    if (!interactiveMode) {
        fclose(inputStream);
    }

    free(command);

    return 0;
}

