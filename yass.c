#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
extern char **environ;

char* my_strtok(char* str, const char* delimiters) {
    static char* nextToken = NULL;
    char* tokenStart;

    if (str != NULL) {
        nextToken = str;
    }

    if (nextToken == NULL) {
        return NULL;
    }

    while (*nextToken != '\0' && strchr(delimiters, *nextToken) != NULL) {
        nextToken++;
    }

    if (*nextToken == '\0') {
        return NULL; 
    }

    tokenStart = nextToken;

    while (*nextToken != '\0' && strchr(delimiters, *nextToken) == NULL) {
        nextToken++;
    }

    if (*nextToken != '\0') {
        *nextToken = '\0'; 
        nextToken++;
    }

    return tokenStart;
}

void tokenizeCommand(char* command, char*** args, int* argCount) {
    char* token;
    int argIndex = 0;

    token = my_strtok(command, " \t\n");
    if (token == NULL) {
        *argCount = 0;
        return;
    }
    (*args)[argIndex] = strdup(token);
    argIndex++;

    while (token != NULL && argIndex < MAX_ARGS - 1) {
        token = my_strtok(NULL, " \t\n");
        if (token != NULL) {
            (*args)[argIndex] = strdup(token);
            argIndex++;
        }
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
    char* path , *pathEnv, *commandPath , * token ;
    int status;

    if (strchr(args[0], '/') != NULL) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execve(args[0], args, environ) == -1) {
                fprintf(stderr, "%s: %d: %s: command not found\n", executableName, commandNumber, args[0]);
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
    } else {
         pathEnv = getenv("PATH");
        if (pathEnv == NULL) {
            fprintf(stderr, "Failed to get the PATH environment variable\n");
            return;
        }

        path = strdup(pathEnv);
        if (path == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        token = strtok(path, ":");
        while (token != NULL) {
             commandPath = (char*)malloc((strlen(token) + strlen(args[0]) + 2) * sizeof(char));
            if (commandPath == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }

            sprintf(commandPath, "%s/%s", token, args[0]);

            pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                if (execve(commandPath, args, environ) == -1) {
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

            free(commandPath);

            token = strtok(NULL, ":");
        }

        fprintf(stderr, "%s: %d: %s: command not found\n", executableName, commandNumber, args[0]);
        free(path);
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
    size_t bufferSize = MAX_COMMAND_LENGTH;
    ssize_t lineSize;
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

        command = (char*)malloc(bufferSize * sizeof(char));
        if (command == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        lineSize = getline(&command, &bufferSize, inputStream);
        if (lineSize == -1) {
            printf("\n");
            break;
        }
        command[lineSize - 1] = '\0';

        if (strcmp(command, "exit") == 0) {
            free(command);
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
        free(command);
        command = NULL;
    }

    if (!interactiveMode) {
        fclose(inputStream);
    }

    return 0;
}

