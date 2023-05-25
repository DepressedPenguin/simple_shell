#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100

void tokenizeCommand(char *command, char ***args, int *argCount);

void freeArgs(char **args);

void executeCommand(char **args, const char *executableName, int commandNumber);

void printPrompt();

char *_getenv(const char *name);

char *readLine();

extern char **environ;

int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH];
    char **args = NULL;
    int argCount = 0;
    int commandNumber = 0;

    FILE *inputStream = stdin;
    int interactiveMode = 1;

    if (argc > 1)
    {
        inputStream = fopen(argv[1], "r");
        interactiveMode = 0;
        if (inputStream == NULL)
        {
            perror("Failed to open the script file");
            exit(EXIT_FAILURE);
        }
    }

    while (1)
    {
        if (isatty(0))
        {
            printPrompt();
        }

        if (fgets(command, sizeof(command), inputStream) == NULL)
        {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        commandNumber++;

        args = (char **)malloc((MAX_ARGS + 1) * sizeof(char *));
        if (args == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        tokenizeCommand(command, &args, &argCount);
        executeCommand(args, argv[0], commandNumber);
        freeArgs(args);
    }

    if (!interactiveMode)
    {
        fclose(inputStream);
    }

    return 0;
}

void tokenizeCommand(char *command, char ***args, int *argCount)
{
    char *token;
    int argIndex = 0;

    token = strtok(command, " \t\n");
    while (token != NULL && argIndex < MAX_ARGS - 1)
    {
        (*args)[argIndex] = strdup(token);
        argIndex++;
        token = strtok(NULL, " \t\n");
    }
    (*args)[argIndex] = NULL;
    *argCount = argIndex;
}

void freeArgs(char **args)
{
    int i;
    for (i = 0; args[i] != NULL; i++)
    {
        free(args[i]);
    }
    free(args);
}

void executeCommand(char **args, const char *executableName, int commandNumber)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "%s: %d: %s: not found\n", executableName, commandNumber, args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        do
        {
            pid_t wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1)
            {
                perror("Waitpid failed");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void printPrompt()
{
    printf("$ ");
    fflush(stdout);
}

char *_getenv(const char *name)
{
    int i;
    size_t len = strlen(name);
    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(name, environ[i], len) == 0 && environ[i][len] == '=')
        {
            return environ[i] + len + 1;
        }
    }
    return NULL;
}

char *readLine()
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

