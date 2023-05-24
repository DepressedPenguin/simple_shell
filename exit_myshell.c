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
