#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **yass_zak;

void ryn_myshell(void);
void exes_cmd(char **args);
void exes_myexev(char **args);
char **fuc_args(char *linee);
int fuc_bul(char **args);
void exe_built(char **args);
void free_myarg(char **args);
void print_prompt(void);
void myp_id(void);
void print_fuid(void);
void print_mstat(char *file_name);
char *_getenv(const char *getname);
char *get_line(void);

int main(int argc __attribute__((unused)), char **argv)
{
    yass_zak = argv;
    ryn_myshell();
    return 0;
}

char *_getenv(const char *getname)
{
    size_t getvlen = strlen(getname);
    char **vilen;

    for (vilen = environ; *vilen != NULL; vilen++)
    {
        if (strncmp(getname, *vilen, getvlen) == 0 && (*vilen)[getvlen] == '=')
        {
            return &((*vilen)[getvlen + 1]);
        }
    }
    return NULL;
}

void print_mstat(char *file_name)
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

void ryn_myshell(void)
{
    char *linee = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        if (isatty(0))
            print_prompt();
        read = getline(&linee, &len, stdin);
        if (read == -1)
        {
            free(linee);
            exit(0);
        }
        args = fuc_args(linee);
        if (!args[0])
        {
            free_myarg(args);
            continue;
        }
        if (fuc_bul(args))
        {
            exe_built(args);
        }
        else
        {
            exes_cmd(args);
        }
        free_myarg(args);
    }
    free(linee);
}

void free_myarg(char **args)
{
    int i;
    for (i = 0; args[i]; i++)
    {
        free(args[i]);
    }
    free(args);
}


void exe_built(char **args)
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
        int curva = STDOUT_FILENO;
        for (env = environ; *env != NULL; env++)
        {
            len = strlen(*env);
            if (write(curva, *env, len) != (ssize_t)len || write(curva, "\n", 1) != (ssize_t)1)
            {
                perror("write");
                break;
            }
        }
    }
}



int fuc_bul(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}


char **fuc_args(char *linee)
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


void print_prompt(void)
{
    write(STDOUT_FILENO, "#yassine$ ", 9);
}

void myp_id(void)
{
    char pid_str[100];
    int pid_len;
    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_len);
}

void print_fuid(void)
{
    char ppid_str[100];
    int ppid_len;
    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_len);
}

void exes_cmd(char **args)
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
                char **cmd_args = fuc_args(cmd);
                exes_myexev(cmd_args);
                free_myarg(cmd_args);
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

 

void exes_myexev(char **args)
{
    char *d, *cmdpvth, *my_pathy, *commy;
    char *err_msg;
    size_t len;
    int i;

    for (i = 0; args[i]; i++)
    {
        commy = args[i];

        if (access(commy, X_OK) == 0)
        {
            execve(commy, args, environ);
        }
        my_pathy = _getenv("PATH");
        d = strtok(my_pathy, ":");

        while (d != NULL)
        {
            cmdpvth = malloc(strlen(d) + strlen(commy) + 2);
            sprintf(cmdpvth, "%s/%s", d, commy);

            if (access(cmdpvth, X_OK) == 0)
            {
                execve(cmdpvth, args, environ);
            }

            free(cmdpvth);
            d = strtok(NULL, ":");
        }
    }

    len = strlen(commy) + 22;
    err_msg = malloc(len);
    sprintf(err_msg, "not found: %s\n", commy);
    write(STDERR_FILENO, err_msg, len);
    free(err_msg);
    exit(127);
}


