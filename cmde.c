#include "yzshell.h"
/**
 * exes_cmdd - Execute a command represented by args.
 * @args: An array of strings representing.
 */

void exes_cmdd(char **args)
{
pid_t iid = fork();
if (iid == 0)
{
com_execve(args);
exit(0);
}
else
{
waitpid(iid, NULL, 0);
}
}
/**
 * get_ly - Read a line from standard input.
 * Return: A pointer to the dynamically allocated.
 */
char *get_ly(void)
{
static int lenn;
static int lenn1;
static char buffer[1024];
char *liine;
int liine_lenn = 0;
if (lenn1 >= lenn)
{
lenn = read(STDIN_FILENO, buffer, 1024);
if (lenn <= 0)
{
return (NULL);
}
lenn1 = 0;
}
liine = malloc(1024);
while (lenn1 < lenn)
{
if (buffer[lenn1] == '\n')
{
lenn1++;
break;
}
liine[liine_lenn++] = buffer[lenn1++];
}
liine[liine_lenn] = '\0';
return (liine);
}
/**
 * com_execve - Execute the command represented
 * @args: An array of strings.
 */
void com_execve(char **args)
{
char *d, *cmdmypath, *pth, *cmdyyd;
char *error_msg;
size_t len;
int i;

for (i = 0; args[i]; i++)
{
cmdyyd = args[i];

if (access(cmdyyd, X_OK) == 0)
{
execve(cmdyyd, args, environ);
}
pth = _getenv("PATH");
d = strtok(pth, ":");

while (d != NULL)
{
cmdmypath = malloc(strlen(d) + strlen(cmdyyd) + 2);
sprintf(cmdmypath, "%s/%s", d, cmdyyd);

if (access(cmdmypath, X_OK) == 0)
{
execve(cmdmypath, args, environ);
}

free(cmdmypath);
d = strtok(NULL, ":");
}
}

len = strlen(cmdyyd) + 22;
error_msg = malloc(len);
sprintf(error_msg, "not found: %s\n", cmdyyd);
write(STDERR_FILENO, error_msg, len);
free(error_msg);
exit(127);
}
