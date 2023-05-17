#include "myshell.h"

/*
 * exes_cmds - run commands with the specified parameters
 *
 * @args: command and its arguments are contained in an array of strings.
 */
void exes_cmds(char **args)
{
char *cmd_path;
pid_t pds = fork();

if (pds == 0)
{
	/* check if command has an absolute path */
if (args[0][0] == '/')
{
execve(args[0], args, NULL);
}
else
{
	/* search for command in directories listed in PATH */
cmd_path = find_wayy(args[0]);

if (cmd_path == 0)
{
printf("%s: No such file or directory\n", exe_name_cmd[0]);
exit(1);
}

execve(cmd_path, args, NULL);
}

	/* print error message if command execution fails */
printf("%s: failed to execute\n", args[0]);
exit(1);

}
else if (pds > 0)
{
	/* wait for child process to complete */
wait(NULL);

}
else
{
	/* exit if fork() failed */
exit(1);
}
}

