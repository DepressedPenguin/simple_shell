#include "myshell.h"
#include <ctype.h>

/**
 * main - Entry point for the program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 * Return: alawys 0
*/
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
    return (0);
}

/**
 * read_stdin - Tokenizes the input string and stores tokens.
 * @input: The input string to tokenize.
 *
 * Return: a collection of strings containing the tokens, or NULL
 */
char **read_stdin(char *input)
{
char **argz = malloc(sizeof(char *) * MAX_ARZ);
char *toksign;
int v = 0;

if (argz == NULL)
{
return (NULL);
}

toksign = strtok(input, " ");

for (v = 0; toksign != NULL; v++)
{
if (v >= MAX_ARZ - 1)
{
return (NULL);
}

argz[v] = malloc(sizeof(char) * MAXA_LEN);
if (argz[v] == NULL)
{
return (NULL);
}

strcpy(argz[v], toksign);

toksign = strtok(NULL, " ");
}

argz[v] = NULL;

return (argz);
}

/**
 * find_wayy - Searches for the specified command.
 * @commands: The command to search for.
 *
 * Return: The full path of the command if found, NULL otherwise.
 */
char *find_wayy(char *commands)
{
char *wayy = getenv("PATH");
char *dr;
char *fulls_canal = malloc(sizeof(char) * (MAXC_LEN + 1));

if (fulls_canal == NULL)
{
return (NULL);
}

dr = strtok(wayy, ":");
while (dr != NULL)
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
/**
 * exit_myshell- function that exits the program
 * @args: arguments
 * Return: None
 */

int should_exit = 0;  

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

    should_exit = 1;
    exit(exit_code);
}


/**
 * change_directory - to Change the current working directory
 * @args: An array of arguments
 */
void change_directory(char **args)
{
char *new_directory;
char *previous_directory = current_directory;
if (args[1] == NULL || strcmp(args[1], "~") == 0)
{
new_directory = getenv("HOME");
}
else if (strcmp(args[1], "-") == 0)
{
new_directory = getenv("OLDPWD");
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
/**
 * printing_env - Print all environment variables.
 */
void printing_env(void)
{
char **env = environ;
while (*env)
{
printf("%s\n", *env);
env++;
}
}


/**
 * _getly - a line is read from a file stream.
 * @lineptr: the buffer where the line is saved, indicated by a pointer
 * @n: pointer to the buffer's size
 * @stream: stream of files to read from
 *
 * Return: number of characters read, -1 on failure
 */
ssize_t _getly(char **lineptr, size_t *n, FILE *stream)
{
ssize_t num_chars = 0;
ssize_t bufsize = 0;
int c;

        /* check for invalid parameters */
if (lineptr == NULL || n == NULL || stream == NULL)
return (-1);

        /* allocate buffer if not already allocated */
if (*lineptr == NULL || *n == 0)
{
*lineptr = malloc(128);
if (*lineptr == NULL)
return (-1);
*n = 128;
}

        /* read characters until end of line or end of file */
while ((c = fgetc(stream)) != '\n' && c != EOF)
{
                /* store character in buffer */
(*lineptr)[num_chars] = c;
num_chars++;

                /* resize buffer if necessary */
if ((size_t)num_chars >= *n)
{
bufsize = *n + 128;
*lineptr = realloc(*lineptr, bufsize);
if (*lineptr == NULL)
return (-1);
*n = bufsize;
}
}

        /* terminate string with null character */
(*lineptr)[num_chars] = '\0';

return (num_chars);
}

/**
 * exes_cmds - Run commands with the specified parameters.
 *
 * @args: Command and its arguments are contained in an array of strings.
 */
void exes_cmds(char **args)
{
char error_msg[64];
char *cmd_path;
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
