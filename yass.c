#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **xd;

void myrr(void);
void exct_my_cmd(char **args);
void exe_cv(char **args);
char **par_f(char *myline);
int built_f(char **args);
void execute_b(char **args);
void fre_argm(char **args);
void print_myprompt(void);
void pntint_cid(void);
void pnty_cid(void);
void print_filestate(char *file_name);
char *_getenv(const char *getname);
char *get_line(void);

int main(int argc __attribute__((unused)), char **argv)
{
    xd = argv;
    myrr();
    return 0;
}

char *_getenv(const char *getname)
{
    size_t mylen = strlen(getname);
    char **varb;

    for (varb = environ; *varb != NULL; varb++)
    {
        if (strncmp(getname, *varb, mylen) == 0 && (*varb)[mylen] == '=')
        {
            return &((*varb)[mylen + 1]);
        }
    }
    return NULL;
}

void print_filestate(char *file_name)
{
    struct stat myfile_state;
    char buffer[1024];
    int len;
    if (stat(file_name, &myfile_state) == -1)
    {
        perror("stat");
        return;
    }
    len = sprintf(buffer, "File: %s\n", file_name);
    write(STDOUT_FILENO, buffer, len);
}

#define BUFF_SSIZE 4096
#define IMAX_ARGS 258

void myrr(void)
{
    char *myline = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        if (isatty(0))
            print_myprompt();
        read = getline(&myline, &len, stdin);
        if (read == -1)
        {
            free(myline);
            exit(0);
        }
        args = par_f(myline);
        if (!args[0])
        {
            fre_argm(args);
            continue;
        }
        if (built_f(args))
        {
            execute_b(args);
        }
        else
        {
            exct_my_cmd(args);
        }
        fre_argm(args);
    }
    free(myline);
}

void fre_argm(char **args)
{
    int i;
    for (i = 0; args[i]; i++)
    {
        free(args[i]);
    }
    free(args);
}


void execute_b(char **args)
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
        const char *yass  = args[1] ? args[1] : _getenv("HOME");
        if (chdir(yass ) != 0)
        {
            perror("cd");
        }
    }
    else if (strcmp(args[0], "env") == 0)
    {
        char **env;
        ssize_t len;
        int rh = STDOUT_FILENO;
        for (env = environ; *env != NULL; env++)
        {
            len = strlen(*env);
            if (write(rh, *env, len) != (ssize_t)len || write(rh, "\n", 1) != (ssize_t)1)
            {
                perror("write");
                break;
            }
        }
    }
}



int built_f(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}


char **par_f(char *myline)
{
    char **args = malloc(IMAX_ARGS * sizeof(char *));
    char *tkn;
    int i = 0;
    tkn = strtok(myline, " \t\n;");
    while (tkn != NULL && i < IMAX_ARGS)
    {
        args[i++] = strdup(tkn);
        tkn = strtok(NULL, " \t\n;");
    }
    args[i] = NULL;
    return args;
}


void print_myprompt(void)
{
    write(STDOUT_FILENO, "#yassine$ ", 9);
}

void pntint_cid(void)
{
    char pid_str[100];
    int pid_len;
    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_len);
}

void pnty_cid(void)
{
    char ppid_str[100];
    int ppid_len;
    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_len);
}

void exct_my_cmd(char **args)
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
                char **cmd_args = par_f(cmd);
                exe_cv(cmd_args);
                fre_argm(cmd_args);
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

 

void exe_cv(char **args)
{
    char *d, *mycmd_path, *ppath, *my_cmd;
    char *err_m;
    size_t len;
    int i;

    for (i = 0; args[i]; i++)
    {
        my_cmd = args[i];

        if (access(my_cmd, X_OK) == 0)
        {
            execve(my_cmd, args, environ);
        }
        ppath = _getenv("PATH");
        d = strtok(ppath, ":");

        while (d != NULL)
        {
            mycmd_path = malloc(strlen(d) + strlen(my_cmd) + 2);
            sprintf(mycmd_path, "%s/%s", d, my_cmd);

            if (access(mycmd_path, X_OK) == 0)
            {
                execve(mycmd_path, args, environ);
            }

            free(mycmd_path);
            d = strtok(NULL, ":");
        }
    }

    len = strlen(my_cmd) + 22;
    err_m = malloc(len);
    sprintf(err_m, "not found: %s\n", my_cmd);
    write(STDERR_FILENO, err_m, len);
    free(err_m);
    exit(127);
}


