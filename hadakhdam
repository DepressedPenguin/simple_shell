#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **zak;

void run_mycode(void);
void exes_cmdd(char **args);
void com_execve(char **args);
char **pars_args(char *linP);
int fun_bult(char **args);
void exec_bultts(char **args);
void free_argsss(char **args);
void print_out(void);
void print_myid(void);
void print_cid(void);
void printstt(char *F_name);
char *_getenv(const char *name);
char *get_ly(void);

int main(int argc __attribute__((unused)), char **argv)
{
    zak = argv;
    run_mycode();
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

#define BUFF_SSIZE 4096
#define IMAX_ARGS 258

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

void free_argsss(char **args)
{
    int i;
    for (i = 0; args[i]; i++)
    {
        free(args[i]);
    }
    free(args);
}


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



int fun_bult(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}

char **pars_args(char *linP)
{
    char **args = malloc(IMAX_ARGS * sizeof(char *));
    char *tkn;
    int i = 0;
    tkn = strtok(linP, " \t\n");
    while (tkn != NULL && i < IMAX_ARGS)
    {
        args[i++] = strdup(tkn);
        tkn = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return args;
}

void print_out(void)
{
    write(STDOUT_FILENO, "#yassine$ ", 9);
}

void print_myid(void)
{
    char pid_str[100];
    int pid_len;
    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_len);
}

void print_cid(void)
{
    char ppid_str[100];
    int ppid_len;
    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_len);
}

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
            return NULL;
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
    return liine;
}

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

