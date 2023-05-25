#include "yzshell.h"
/**
 * _getenv - Get the value of an environment
 * @name: The name of the environment
 * returns its value.
 * Return: A pointer to the value of the environment
 */
char *_getenv(const char *name)
{
size_t nvmelen = strlen(name);
char **var;

for (var = environ; *var != NULL; var++)
{
if (strncmp(name, *var, nvmelen) == 0 && (*var)[nvmelen] == '=')
{
return (&((*var)[nvmelen + 1]));
}
}
return (NULL);
}
/**
 * printstt - Print file information using stat.
 * @F_name: The name of the file.
 */
void printstt(char *F_name)
{
struct stat file_stat;
char buffer[1024];
int len;
if (stat(F_name, &file_stat) == -1)
{
perror("stat");
return;
}
len = sprintf(buffer, "File: %s\n", F_name);
write(STDOUT_FILENO, buffer, len);
}
/**
 * run_mycode - Run the shell code.
 */
void run_mycode(void)
{
char *linP = NULL;
char **args;
size_t len = 0;
ssize_t read;
while (1)
{
if (isatty(0))
print_out();
read = getline(&linP, &len, stdin);
if (read == -1)
{
free(linP);
exit(0);
}
args = pars_args(linP);
if (!args[0])
{
free_argsss(args);
continue;
}
if (fun_bult(args))
{
exec_bultts(args);
}
else
{
exes_cmdd(args);
}
free_argsss(args);
}
free(linP);
}
/**
 * free_argsss - Free the memory allocated.
 * @args: The array of argument strings.
 */
void free_argsss(char **args)
{
int i;
for (i = 0; args[i]; i++)
{
free(args[i]);
}
free(args);
}
/**
 * exec_bultts - Execute built-in shell commands.
 * @args: The array of command arguments.
 */
void exec_bultts(char **args)
{
if (!args || !args[0])
{
return;
}
if (strcmp(args[0], "exit") == 0)
{
int status = 0;
if (args[1])
{
status = atoi(args[1]);
}
exit(status);
}
else if (strcmp(args[0], "cd") == 0)
{
const char *di = args[1] ? args[1] : _getenv("HOME");
if (chdir(di) != 0)
{
perror("cd");
}
}
else if (strcmp(args[0], "env") == 0)
{
char **env;
ssize_t len;
int sf = STDOUT_FILENO;
for (env = environ; *env != NULL; env++)
{
len = strlen(*env);
if (write(sf, *env, len) != (ssize_t)len || write(sf, "\n", 1) != (ssize_t)1)
{
perror("write");
break;
}
}
}
}
