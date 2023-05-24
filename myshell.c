#include "myshell.h"

int should_exit = 0;
char **exe_name_cmd;
char *current_directory;

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
    char *wayy = _getenv("PATH");
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

int main(__attribute__((unused)) int argc, char *argv[])
{
    char *stdput = NULL;
    size_t stdput_leng = 0;
    char **args;
    exe_name_cmd = argv;
    current_directory = getcwd(NULL, 0);
    setenv("PWD", current_directory, 1);

    while (!should_exit)
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

