#include "myshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64
#define MAX_ARGUMENT_LENGTH 128

char **read_stdin(char *input) {
    char **arguments = malloc(sizeof(char*) * MAX_ARGUMENTS);
    char *token;
    int i = 0;
    if (arguments == NULL) {
        return (NULL);
    }

 token = strtok(input, " ");

    while (token != NULL) {
        if (i >= MAX_ARGUMENTS - 1) {
            return (NULL);
        
	    /* nrj3o lih mn b3d hh 
	     */
        }

        arguments[i] = malloc(sizeof(char) * MAX_ARGUMENT_LENGTH);
        if (arguments[i] == NULL) {
             return (NULL);
        }

        strcpy(arguments[i], token);

        token = strtok(NULL, " ");
        i++;
    }

    arguments[i] = NULL;

    return arguments;
}

void execute_command(char** args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        printf("No such file or directory\n");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {

        exit(1);
    }
}

void exit_shell() {
    printf("Exiting shell...\n");
    exit(0);
}

void print_env() {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char **args;

    while (1) {
     
        printf("$ ");
        fgets(input, MAX_COMMAND_LENGTH, stdin);
        input[strcspn(input, "\n")] = 0;

        
        args = read_stdin(input);

        
        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                exit_shell();
            } else if (strcmp(args[0], "env") == 0) {
                print_env();
            } else {
                execute_command(args);
            }
        }
    }

    return 0;
}

