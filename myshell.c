#include "myshell.h"

void free_tokens(char **argz)
{
    int i = 0;
    while (argz[i] != NULL)
    {
        free(argz[i]);
        i++;
    }
    free(argz);
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
            free_tokens(argz);
            return NULL;
        }

        argz[v] = malloc(sizeof(char) * MAXA_LEN);
        if (argz[v] == NULL)
        {
            free_tokens(argz);
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

