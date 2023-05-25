#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **custom_argv;

void run_shell(void);
void execute_command(char **args);
void execute_execve(char **args);
char **parse_args(char *line);
int is_builtin(char **args);
void execute_builtin(char **args);
void free_args(char **args);
void print_prompt(void);
void print_pid(void);
void print_ppid(void);
void print_file_stat(char *file_name);
char *_custom_getenv(const char *name);
char *get_input_line(void);

int main(int argc __attribute__((unused)), char **argv)
{
    custom_argv = argv;
    run_shell();
    return 0;
}

char *_custom_getenv(const char *name)
{
    size_t name_length = strlen(name);
    char **var;

    for (var = environ; *var != NULL; var++)
    {
        if (strncmp(name, *var, name_length) == 0 && (*var)[name_length] == '=')
        {
            return &((*var)[name_length + 1]);
        }
    }
    return NULL;
}

void print_file_stat(char *file_name)
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

#define BUFFER_SIZE 4096
#define MAX_ARGS 258

void run_shell(void)
{
    char *input_line = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        if (isatty(0))
            print_prompt();
        read = getline(&input_line, &len, stdin);
        if (read == -1)
        {
            free(input_line);
            exit(0);
        }
        args = parse_args(input_line);
        if (!args[0])
        {
            free_args(args);
            continue;
        }
        if (is_builtin(args))
        {
            execute_builtin(args);
        }
        else
        {
            execute_command(args);
        }
        free_args(args);
    }
    free(input_line);
}

void free_args(char **args)
{
    int i;
    for (i = 0; args[i]; i++)
    {
        free(args[i]);
    }
    free(args);
}

void execute_builtin(char **args)
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
        const char *dir = args[1] ? args[1] : _custom_getenv("HOME");
        if (chdir(dir) != 0)
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

int is_builtin(char **args)
{
    return (strcmp(args[0], "exit") == 0 ||
            strcmp(args[0], "cd") == 0 ||
            strcmp(args[0], "env") == 0);
}

char **parse_args(char *line)
{
    char **args = malloc(MAX_ARGS * sizeof(char *));
    char *token;
    int i = 0;
    token = strtok(line, " \t\n;");
    while (token != NULL && i < MAX_ARGS)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " \t\n;");
    }
    args[i] = NULL;
    return args;
}

void print_prompt(void)
{
    write(STDOUT_FILENO, "#yassine$ ", 10);
}

void print_pid(void)
{
    char pid_str[100];
    int pid_len;
    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_len);
}

void print_ppid(void)
{
    char ppid_str[100];
    int ppid_len;
    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_len);
}

void execute_command(char **args)
{
    int i = 0;
    while (args[i])
    {
        char *cmd = args[i];
        if (strcmp(cmd, ";") != 0)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                char **cmd_args = parse_args(cmd);
                execute_execve(cmd_args);
                free_args(cmd_args);
                exit(0);
            }
            else
            {
                waitpid(pid, NULL, 0);
            }
        }
        i++;
    }
}

void execute_execve(char **args)
{
    char *dir, *cmd_path, *path, *command;
    char *error_msg;
    size_t len;
    int i;

    for (i = 0; args[i]; i++)
    {
        command = args[i];

        if (access(command, X_OK) == 0)
        {
            execve(command, args, environ);
        }
        path = _custom_getenv("PATH");
        dir = strtok(path, ":");

        while (dir != NULL)
        {
            cmd_path = malloc(strlen(dir) + strlen(command) + 2);
            sprintf(cmd_path, "%s/%s", dir, command);

            if (access(cmd_path, X_OK) == 0)
            {
                execve(cmd_path, args, environ);
            }

            free(cmd_path);
            dir = strtok(NULL, ":");
        }
    }

    len = strlen(command) + 22;
    error_msg = malloc(len);
    sprintf(error_msg, "not found: %s\n", command);
    write(STDERR_FILENO, error_msg, len);
    free(error_msg);
    exit(127);
}

