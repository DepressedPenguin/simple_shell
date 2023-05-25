#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **hd;

void irun_shell(void);
void exct_commvnd(char **args);
void com_execve(char **args);
char **thom_args(char *linee);
int thom_builtin(char **args);
void execute_builtinthom(char **args);
void thomfree_args(char **args);
void thom_prompt(void);
void pnt_iid(void);
void pnt_iiid(void);
void prt_fle_stt(char *file_name);
char *_getenv(const char *name);
char *get_line(void);

int main(int argc __attribute__((unused)), char **argv)
{
    hd = argv;
    irun_shell();
    return 0;
}

char *_getenv(const char *name)
{
    size_t nvmelen = strlen(name);
    char **var;

    for (var = environ; *var != NULL; var++)
    {
        if (strncmp(name, *var, nvmelen) == 0 && (*var)[nvmelen] == '=')
        {
            return &((*var)[nvmelen + 1]);
        }
    }
    return NULL;
}

void prt_fle_stt(char *file_name)
{
    struct stat file_stat;
    char buffer[1024];
    int len;
    if (stat(file_name, &file_stat) == -1)
    {
        perror("stat");
        return;
    }
    len = sprintf(buffer, "File: %s\n", file_name);
    write(STDOUT_FILENO, buffer, len);
}

#define BUFF_SSIZE 4096
#define IMAX_ARGS 258

void irun_shell(void)
{
    char *linee = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        if (isatty(0))
            thom_prompt();
        read = getline(&linee, &len, stdin);
        if (read == -1)
        {
            free(linee);
            exit(0);
        }
        args = thom_args(linee);
        if (!args[0])
        {
            thomfree_args(args);
            continue;
        }
        if (thom_builtin(args))
        {
            execute_builtinthom(args);
        }
        else
        {
            exct_commvnd(args);
        }
        thomfree_args(args);
    }
    free(linee);
}

void thomfree_args(char **args)
{
    int i;
    for (i = 0; args[i]; i++)
    {
        free(args[i]);
    }
    free(args);
}


void execute_builtinthom(char **args)
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



int thom_builtin(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}


char **thom_args(char *linee)
{
    char **args = malloc(IMAX_ARGS * sizeof(char *));
    char *tkn;
    int i = 0;
    tkn = strtok(linee, " \t\n;");
    while (tkn != NULL && i < IMAX_ARGS)
    {
        args[i++] = strdup(tkn);
        tkn = strtok(NULL, " \t\n;");
    }
    args[i] = NULL;
    return args;
}


void thom_prompt(void)
{
    write(STDOUT_FILENO, "#yassine$ ", 9);
}

void pnt_iid(void)
{
    char pid_str[100];
    int pid_len;
    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_len);
}

void pnt_iiid(void)
{
    char ppid_str[100];
    int ppid_len;
    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_len);
}

void exct_commvnd(char **args)
{
    int i = 0;
    while (args[i])
    {
        char *cmd = args[i];
        if (strcmp(cmd, ";") != 0)
        {
            pid_t iid = fork();
            if (iid == 0)
            {
                char **cmd_args = thom_args(cmd);
                com_execve(cmd_args);
                thomfree_args(cmd_args);
                exit(0);
            }
            else
            {
                waitpid(iid, NULL, 0);
            }
        }
        i++;
    }
}

 

void com_execve(char **args)
{
    char *d, *cmdpvth, *pth, *commvnd;
    char *error_msg;
    size_t len;
    int i;

    for (i = 0; args[i]; i++)
    {
        commvnd = args[i];

        if (access(commvnd, X_OK) == 0)
        {
            execve(commvnd, args, environ);
        }
        pth = _getenv("PATH");
        d = strtok(pth, ":");

        while (d != NULL)
        {
            cmdpvth = malloc(strlen(d) + strlen(commvnd) + 2);
            sprintf(cmdpvth, "%s/%s", d, commvnd);

            if (access(cmdpvth, X_OK) == 0)
            {
                execve(cmdpvth, args, environ);
            }

            free(cmdpvth);
            d = strtok(NULL, ":");
        }
    }

    len = strlen(commvnd) + 22;
    error_msg = malloc(len);
    sprintf(error_msg, "not found: %s\n", commvnd);
    write(STDERR_FILENO, error_msg, len);
    free(error_msg);
    exit(127);
}

