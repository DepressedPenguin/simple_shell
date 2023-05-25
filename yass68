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
void exct_command(char **args);
void com_execve(char **args);
char **parse_args(char *line);
int is_builtin(char **args);
void execute_builtin(char **args);
void free_args(char **args);
void display_prompt(void);
void print_pid(void);
void print_ppid(void);
void print_file_stat(char *file_name);
char *_getenv(const char *name);
char *get_line(void);

#define BUFFER_SIZE 4096
#define MAX_ARGS 258

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

void irun_shell(void)
{
    char *line = NULL;
    char **args;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(0);
        }

        args = parse_args(line);
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
            exct_command(args);
        }

        free_args(args);
    }

    free(line);
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
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        const char *dir = args[1] ? args[1] : _getenv("HOME");
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
            if (write(sf, *env, len) != len || write(sf, "\n", 1) != 1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int is_builtin(char **args)
{
    return strcmp(args[0], "exit") == 0 ||
           strcmp(args[0], "cd") == 0 ||
           strcmp(args[0], "env") == 0;
}

char **parse_args(char *line)
{
    char **args = malloc(MAX_ARGS * sizeof(char *));
    char *token;
    int i = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_ARGS)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    return args;
}

void display_prompt(void)
{
    if (write(STDOUT_FILENO, "# ", 2) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

void print_pid(void)
{
    char pid_str[100];
    int pid_len;

    pid_len = sprintf(pid_str, "PID: %d\n", getpid());
    if (write(STDOUT_FILENO, pid_str, pid_len) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

void print_ppid(void)
{
    char ppid_str[100];
    int ppid_len;

    ppid_len = sprintf(ppid_str, "PPID: %d\n", getppid());
    if (write(STDOUT_FILENO, ppid_str, ppid_len) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
}

void exct_command(char **args)
{
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0)
    {
        com_execve(args);
        exit(0);
    }
    else
    {
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
}

char *get_line(void)
{
    static int len;
    static int len1;
    static char buffer[BUFFER_SIZE];
    char *line;
    int line_len = 0;

    if (len1 >= len)
    {
        len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (len == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (len == 0)
        {
            return NULL;
        }
        len1 = 0;
    }

    line = malloc(BUFFER_SIZE);
    if (line == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (len1 < len)
    {
        if (buffer[len1] == '\n')
        {
            len1++;
            break;
        }
        line[line_len++] = buffer[len1++];
    }
    line[line_len] = '\0';

    return line;
}

void com_execve(char **args)
{
    char *dir, *cmd_path, *path, *command;
    char *error_msg;
    int i;

    for (i = 0; args[i]; i++)
    {
        command = args[i];

        if (access(command, X_OK) == 0)
        {
            execve(command, args, environ);
        }

        path = _getenv("PATH");
        if (path == NULL)
        {
            path = "";
        }

        dir = strtok(path, ":");
        while (dir != NULL)
        {
            cmd_path = malloc(strlen(dir) + strlen(command) + 2);
            if (cmd_path == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            strcpy(cmd_path, dir);
            strcat(cmd_path, "/");
            strcat(cmd_path, command);

            execve(cmd_path, args, environ);

            free(cmd_path);
            dir = strtok(NULL, ":");
        }
    }

    error_msg = malloc(strlen(command) + 13);
    if (error_msg == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sprintf(error_msg, "%s: command not found\n", command);
    if (write(STDERR_FILENO, error_msg, strlen(error_msg)) == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    free(error_msg);
}

