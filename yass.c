#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;
char **hd;

void runShell();
void executeCommand(char **args);
void executeBuiltinCommand(char **args);
char **tokenizeArgs(char *line);
int isBuiltinCommand(char **args);
void freeArgs(char **args);
void printPrompt();
char *_getenv(const char *name);
char *readLine();

int main(int argc __attribute__((unused)), char **argv)
{
    hd = argv;
    runShell();
    return 0;
}

void runShell()
{
    char *line = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        if (isatty(0))
            printPrompt();

        read = getline(&line, &len, stdin);

        if (read == -1)
        {
            free(line);
            exit(0);
        }

        args = tokenizeArgs(line);

        if (!args[0])
        {
            freeArgs(args);
            continue;
        }

        if (isBuiltinCommand(args))
        {
            executeBuiltinCommand(args);
        }
        else
        {
            executeCommand(args);
        }

        freeArgs(args);
    }

    free(line);
}

void executeCommand(char **args)
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
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
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

void executeBuiltinCommand(char **args)
{
    if (!args || !args[0])
    {
        return;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        const char *dir = args[1] ? args[1] : _getenv("HOME");
        if (chdir(dir) != 0)
        {
            perror("cd");
        }
    }
    else if (strcmp(args[0], "env") == 0)
    {
        char **env;
        for (env = environ; *env != NULL; env++)
        {
            printf("%s\n", *env);
        }
    }
}

int isBuiltinCommand(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}

char **tokenizeArgs(char *line)
{
    char **args = malloc((MAX_ARGS + 1) * sizeof(char *));
    char *token;
    int argIndex = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && argIndex < MAX_ARGS - 1)
    {
        args[argIndex++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }
    args[argIndex] = NULL;

    return args;
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

void printPrompt()
{
    printf("$ ");
    fflush(stdout);
}

char *_getenv(const char *name)
{
    size_t namelen = strlen(name);
    char **var;

    for (var = environ; *var != NULL; var++)
    {
        if (strncmp(name, *var, namelen) == 0 && (*var)[namelen] == '=')
        {
            return &((*var)[namelen + 1]);
        }
    }

    return NULL;
}

char *readLine()
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

