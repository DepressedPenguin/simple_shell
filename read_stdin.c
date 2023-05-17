#include "myshell.h"

/*
 * read_stdin - Tokenizes the input string and stores tokens.
 * @input: The input string to tokenize
 *
 * Return: An array of strings containing the tokens, or NULL on failure
 */
char **read_stdin(char *input)
{
char **argz = malloc(sizeof(char *) * MAX_ARZ);
char *toksign;
int v = 0;

if (argz == 0)
	{
return (NULL);
}

toksign = strtok(input, " ");

for (v = 0; toksign != 0; v++)
{
if (v >= MAX_ARZ - 1)
{
return (NULL);
}

argz[v] = malloc(sizeof(char) * MAXA_LEN);
if (argz[v] == 0)
{
return (NULL);
}

strcpy(argz[v], toksign);

toksign = strtok(NULL, " ");
}

argz[v] = NULL;

return (argz);
}

/*
 * find_wayy - Searches for the specified command.
 * @commands: The command to search for
 *
 * Return: The full path of the command if found, NULL otherwise
 */
char *find_wayy(char *commands)
{
char *wayy = getenv("PATH");
char *dr;
char *fulls_canal = malloc(sizeof(char) * (MAXC_LEN + 1));

if (fulls_canal == 0)
{
return (NULL);
}

dr = strtok(wayy, ":");
while (dr != 0)
{
snprintf(fulls_canal, MAXC_LEN, "%s/%s", dr, commands);

if (access(fulls_canal, X_OK) == 0)
{
return (fulls_canal);
}

dr = strtok(NULL, ":");
}

free(fulls_canal);

return (NULL);
}

