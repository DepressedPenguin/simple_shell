#include "myshell.h"

/**
 * exes_cmds - Run commands with the specified parameters.
 *
 * @args: Command and its arguments are contained in an array of strings.
 */
void exes_cmds(char **args)
{
char *cmd_path;
pid_t pds = fork();

if (pds == 0)
{
if (args[0][0] == '/')
{
execve(args[0], args, NULL);
}
else
{
cmd_path = find_wayy(args[0]);

if (cmd_path == NULL)
{
printf("%s: No such file or directory\n", exe_name_cmd[0]);
exit(1);
}

execve(cmd_path, args, NULL);
}

printf("%s: failed to execute\n", args[0]);
exit(1);
}
else if (pds > 0)
{
wait(NULL);
}
else
{
exit(1);
}
}
