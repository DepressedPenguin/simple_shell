#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;

void run_shell();
void execute_command(char **args);
void execute_builtin(char **args);
char **parse_args(char *line);
int is_builtin(char **args);
void print_environment();

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    run_shell();
    return 0;
}

void run_shell()
{
    char *line = NULL;
    size_t line_len = 0;
    char **args;
    ssize_t read;

    while (1)
    {
        printf("#yassine$ ");

        read = getline(&line, &line_len, stdin);

        if (read == -1)
        {
            free(line);
            exit(0);
        }

        args = parse_args(line);

        if (!args[0])
        {
            free(args);
            continue;
        }

        if (is_builtin(args))
        {
            execute_builtin(args);
        }
        else
        {
            execute_command(args);
        }

        free(args);
    }

    free(line);
}

void execute_command(char **args)
{
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1)
    {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        execvp(args[0], args);
        perror("Execution error");
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(child_pid, &status, 0);
    }
}

void execute_builtin(char **args)
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
        const char *dir = args[1] ? args[1] : getenv("HOME");

        if (chdir(dir) != 0)
        {
            perror("cd");
        }
    }
    else if (strcmp(args[0], "env") == 0)
    {
        print_environment();
    }
}

char **parse_args(char *line)
{
    char **args = malloc(258 * sizeof(char *));
    char *token;
    int i = 0;

    token = strtok(line, " \t\n");

    while (token != NULL && i < 258)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }

    args[i] = NULL;

    return args;
}

int is_builtin(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}

void print_environment()
{
    char **env_var = environ;

    while (*env_var != NULL)
    {
        printf("%s\n", *env_var);
        env_var++;
    }
}

