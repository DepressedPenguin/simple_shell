extern char **environ;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
void tokenizeCommand(char *command, char ***args, int *argCount);
void freeArguments(char **args);
void executeCommand(char **args, const char *executableName, int commandNumber);
char *_getenv(const char *name);
char *readLine();

int main(int argc, char *argv[])
{
    FILE *inputStream = stdin;
    int interactiveMode = 1;
    char command[MAX_COMMAND_LENGTH];
    char **args = NULL;
    int argCount = 0;
    int commandNumber = 0;
    char *line;
    pid_t pid;
    int status;

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
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        line = readLine();
        if (line == NULL)
        {
            printf("\n");
            break;
        }

        strcpy(command, line);
        free(line);

        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "env") == 0)
        {
            char **env = environ;
            while (*env)
            {
                printf("%s\n", *env++);
            }
            continue;
        }

        commandNumber++;

        args = (char **)malloc((MAX_ARGS + 1) * sizeof(char *));
        if (args == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        tokenizeCommand(command, &args, &argCount);

        pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char *executableName = args[0];
            char *envPath = _getenv("PATH");
            char *path = strtok(envPath, ":");

            while (path != NULL)
            {
                char executablePath[MAX_COMMAND_LENGTH];
                strcpy(executablePath, path);
                strcat(executablePath, "/");
                strcat(executablePath, executableName);
                execve(executablePath, args, environ);
                path = strtok(NULL, ":");
            }

            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], commandNumber, executableName);
            exit(EXIT_FAILURE);
        }
        else
        {
            while (wait(&status) != pid)
                ;
        }

        freeArguments(args);
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

void freeArguments(char **args)
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
        execvp(args[0], args);

        fprintf(stderr, "%s: %d: %s: not found\n", executableName, commandNumber, args[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        while (wait(&status) != pid)
            ;
    }
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
    ssize_t bytesRead;

    bytesRead = getline(&line, &bufsize, stdin);
    if (bytesRead == -1)
    {
        free(line);
        return NULL;
    }

    if (bytesRead > 0 && line[bytesRead - 1] == '\n')
    {
        line[bytesRead - 1] = '\0';
    }

    return line;
}

