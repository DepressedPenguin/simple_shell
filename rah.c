#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;

#define MAX_ARZ 258
#define MAXA_LEN 1024
#define MAXC_LEN 1024

char **read_stdin(char *input);
char *find_wayy(char *commands);
void exit_myshell(char **args);
void change_directory(char **args);
void printing_env(void);
void exes_cmds(char **args);

char **exe_name_cmd;
char *current_directory;

int main(__attribute__((unused)) int argc, char *argv[])
{
    char *stdput = NULL;
    size_t stdput_leng = 0;
    char **args;
    exe_name_cmd = argv;
    current_directory = getcwd(NULL, 0);
    setenv("PWD", current_directory, 1);

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);

        getline(&stdput, &stdput_leng, stdin);
        stdput[strcspn(stdput, "\n")] = 0;
        args = read_stdin(stdput);
        if (args[0] != NULL)
        {
            if (strcmp(args[0], "exit") == 0)
            {
                exit_myshell(args);
            }
            else if (strcmp(args[0], "env") == 0)
            {
                printing_env();
            }
            else if (strcmp(args[0], "cd") == 0)
            {
                change_directory(args);
            }
            else
            {
                exes_cmds(args);
            }
        }
    }

    free(stdput);
    free(current_directory);
    return 0;
}

char **read_stdin(char *input)
{
    char **argz = malloc(sizeof(char *) * MAX_ARZ);
    char *toksign;
    int v = 0;

    if (argz == NULL)
    {
        return NULL;
    }

    toksign = strtok(input, " ");

    for (v = 0; toksign != NULL; v++)
    {
        if (v >= MAX_ARZ - 1)
        {
            return NULL;
        }

        argz[v] = malloc(sizeof(char) * MAXA_LEN);
        if (argz[v] == NULL)
        {
            return NULL;
        }

        strcpy(argz[v], toksign);

        toksign = strtok(NULL, " ");
    }

    argz[v] = NULL;

    return argz;
}

char *find_wayy(char *commands)
{
    char *wayy = getenv("PATH");
    char *dr;
    char *fulls_canal = malloc(sizeof(char) * (MAXC_LEN + 1));

    if (fulls_canal == NULL)
    {
        return NULL;
    }

    dr = strtok(wayy, ":");
    while (dr != NULL)
    {
        snprintf(fulls_canal, MAXC_LEN, "%s/%s", dr, commands);

        if (access(fulls_canal, X_OK) == 0)
        {
            return fulls_canal;
        }

        dr = strtok(NULL, ":");
    }

    free(fulls_canal);

    return NULL;
}

void exit_myshell(char **args)
{
    int exit_code = 0;
    if (args[1] != NULL)
    {
        exit_code = atoi(args[1]);
        if (exit_code == 0 && strcmp(args[1], "0") != 0)
        {
            printf("exit: %s: numeric argument required\n", args[1]);
            return;
        }
    }
    free(current_directory);
    free(args);
    exit(exit_code);
}

void change_directory(char **args)
{
    char *new_directory;

    if (args[1] == NULL)
    {
        new_directory = getenv("HOME");
    }
    else if (strcmp(args[1], "-") == 0)
    {
        new_directory = getenv("OLDPWD");
        if (new_directory == NULL)
        {
            printf("cd: OLDPWD not set\n");
            return;
        }
    }
    else
    {
        new_directory = args[1];
    }

    if (chdir(new_directory) != 0)
    {
        printf("cd: %s: No such file or directory\n", new_directory);
        return;
    }

    setenv("OLDPWD", current_directory, 1);
    free(current_directory);
    current_directory = getcwd(NULL, 0);
    setenv("PWD", current_directory, 1);
}

void printing_env(void)
{
    char **env_var = environ;

    while (*env_var != NULL)
    {
        printf("%s\n", *env_var);
        env_var++;
    }
}

void exes_cmds(char **args)
{
    pid_t child_pid;
    int status;
    char *way;

    child_pid = fork();
    if (child_pid == -1)
    {
        printf("Error creating child process\n");
        return;
    }
    if (child_pid == 0)
    {
        way = find_wayy(args[0]);
        if (way == NULL)
        {
            printf("%s: command not found\n", args[0]);
            free(args);
            exit(127);
        }

        execve(way, args, environ);
        printf("Error executing command: %s\n", args[0]);
        free(args);
        free(current_directory);
        exit(126);
    }
    else
    {
        waitpid(child_pid, &status, 0);
    }
}

