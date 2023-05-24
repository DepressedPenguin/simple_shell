#include "myshell.h"

void exit_myshell(char **args)
{
    int exit_code = 0;
    if (args[1] != NULL)
    {
        int i = 0;
        while (args[1][i])
        {
            if (!isdigit(args[1][i++]))
            {
                perror("illegal number");
                return;
            }
        }
        exit_code = atoi(args[1]);
    }

    exit(exit_code);
}

void change_directory(char **args)
{
    char *new_directory;
    char *previous_directory = current_directory;
    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        new_directory = _getenv("HOME");
    }
    else if (strcmp(args[1], "-") == 0)
    {
        new_directory = _getenv("OLDPWD");
    }
    else
    {
        new_directory = args[1];
    }
    if (chdir(new_directory) == 0)
    {
        setenv("OLDPWD", current_directory, 1);
        current_directory = getcwd(NULL, 0);
        setenv("PWD", current_directory, 1);
    }
    else
    {
        printf("cd: %s: No such file or directory\n", new_directory);
    }
    if (previous_directory != NULL)
        free(previous_directory);
}

void printing_env(void)
{
    char **env = environ;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
}

void exes_cmds(char **args)
{
    char *cmd_path;
    char error_msg[64];
    pid_t pds = fork();

    if (pds == 0)
    {
        if (args[0][0] == '/')
        {
            execve(args[0], args, environ);
        }
        else
        {
            cmd_path = find_wayy(args[0]);

            if (cmd_path == NULL)
            {
                snprintf(error_msg, sizeof(error_msg), "%s: No such file or directory\n", exe_name_cmd[0]);
                write(STDERR_FILENO, error_msg, strlen(error_msg));
                free(cmd_path);
                exit(1);
            }

            execve(cmd_path, args, environ);
            free(cmd_path);
        }

        snprintf(error_msg, sizeof(error_msg), "%s: failed to execute\n", args[0]);
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        exit(1);
    }
    else if (pds > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
        exit(1);
    }
}

