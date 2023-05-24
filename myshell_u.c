#include "myshell.h"

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

