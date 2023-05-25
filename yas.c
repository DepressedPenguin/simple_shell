#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char **environ;
char **hd;

void run_shell(void);
void execute_command(char **arguments);
void execute_execve(char **arguments);
char **parse_arguments(char *line);
int is_builtin_command(char **arguments);
void execute_builtin_command(char **arguments);
void free_arguments(char **arguments);
void display_prompt(void);
void print_process_id(void);
void print_parent_process_id(void);
void print_file_status(char *file_name);
char *_get_environment_variable(const char *name);
char *read_line(void);

int main(int argc __attribute__((unused)), char **argv)
{
    hd = argv;
    run_shell();
    return 0;
}

char *_get_environment_variable(const char *name)
{
    size_t name_length = strlen(name);
    char **variable;

    for (variable = environ; *variable != NULL; variable++)
    {
        if (strncmp(name, *variable, name_length) == 0 && (*variable)[name_length] == '=')
        {
            return &((*variable)[name_length + 1]);
        }
    }

    return NULL;
}

void print_file_status(char *file_name)
{
    struct stat file_status;
    char buffer[1024];
    int length;

    if (stat(file_name, &file_status) == -1)
    {
        perror("stat");
        return;
    }

    length = sprintf(buffer, "File: %s\n", file_name);
    write(STDOUT_FILENO, buffer, length);
}

#define BUFFER_SIZE 4096
#define MAX_ARGS 258

void run_shell(void)
{
    char *line = NULL;
    char **arguments;
    size_t length = 0;
    ssize_t read;

    while (1)
    {
        if (isatty(0))
        {
            display_prompt();
        }

        read = getline(&line, &length, stdin);
        if (read == -1)
        {
            free(line);
            exit(0);
        }

        arguments = parse_arguments(line);
        if (!arguments[0])
        {
            free_arguments(arguments);
            continue;
        }

        if (is_builtin_command(arguments))
        {
            execute_builtin_command(arguments);
        }
        else
        {
            execute_command(arguments);
        }

        free_arguments(arguments);
    }

    free(line);
}

void free_arguments(char **arguments)
{
    int i;
    for (i = 0; arguments[i]; i++)
    {
        free(arguments[i]);
    }
    free(arguments);
}

void execute_builtin_command(char **arguments)
{
    if (!arguments || !arguments[0])
    {
        return;
    }

    if (strcmp(arguments[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(arguments[0], "cd") == 0)
    {
        const char *directory = arguments[1] ? arguments[1] : _get_environment_variable("HOME");
        if (chdir(directory) != 0)
        {
            perror("cd");
        }
    }
    else if (strcmp(arguments[0], "env") == 0)
    {
        char **environment_variable;
        ssize_t length;
        int stdout_fd = STDOUT_FILENO;

        for (environment_variable = environ; *environment_variable != NULL; environment_variable++)
        {
            length = strlen(*environment_variable);
            if (write(stdout_fd, *environment_variable, length) != length || write(stdout_fd, "\n", 1) != 1)
            {
                perror("write");
                break;
            }
        }
    }
}

int is_builtin_command(char **arguments)
{
    return (strcmp(arguments[0], "exit") == 0 ||
            strcmp(arguments[0], "cd") == 0 ||
            strcmp(arguments[0], "env") == 0);
}

char **parse_arguments(char *line)
{
    char **arguments = malloc(MAX_ARGS * sizeof(char *));
    char *token;
    int i = 0;

    token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_ARGS)
    {
        arguments[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }

    arguments[i] = NULL;
    return arguments;
}

void display_prompt(void)
{
    write(STDOUT_FILENO, "#taha$ ", 7);
}

void print_process_id(void)
{
    char pid_str[100];
    int pid_length;

    pid_length = sprintf(pid_str, "PID: %d\n", getpid());
    write(STDOUT_FILENO, pid_str, pid_length);
}

void print_parent_process_id(void)
{
    char ppid_str[100];
    int ppid_length;

    ppid_length = sprintf(ppid_str, "PPID: %d\n", getppid());
    write(STDOUT_FILENO, ppid_str, ppid_length);
}

void execute_command(char **arguments)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        execute_execve(arguments);
        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}

char *read_line(void)
{
    static int length = 0;
    static int position = 0;
    static char buffer[BUFFER_SIZE];
    char *line;
    int line_length = 0;

    if (position >= length)
    {
        length = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (length <= 0)
        {
            return NULL;
        }
        position = 0;
    }

    line = malloc(BUFFER_SIZE);
    if (line == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (position < length)
    {
        if (buffer[position] == '\n')
        {
            position++;
            break;
        }
        line[line_length++] = buffer[position++];
    }
    line[line_length] = '\0';
    return line;
}

void execute_execve(char **arguments)
{
    char *directory, *command_path, *path, *command;
    char *error_message;
    size_t length;
    int i;

    for (i = 0; arguments[i]; i++)
    {
        command = arguments[i];

        if (access(command, X_OK) == 0)
        {
            execve(command, arguments, environ);
        }

        path = _get_environment_variable("PATH");
        directory = strtok(path, ":");

        while (directory != NULL)
        {
            command_path = malloc(strlen(directory) + strlen(command) + 2);
            if (command_path == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            strcpy(command_path, directory);
            strcat(command_path, "/");
            strcat(command_path, command);

            if (access(command_path, X_OK) == 0)
            {
                execve(command_path, arguments, environ);
            }

            free(command_path);
            directory = strtok(NULL, ":");
        }

        error_message = malloc(strlen(command) + 18);
        if (error_message == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        length = sprintf(error_message, "%s: command not found\n", command);
        write(STDERR_FILENO, error_message, length);
        free(error_message);
        exit(EXIT_FAILURE);
    }
}

