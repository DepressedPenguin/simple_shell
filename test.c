#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXC_LEN 1024
#define MAX_ARZ 64
#define MAXA_LEN 128

char **exe_name_cmd;
char *current_directory;
void free_char_array(char **array) {
   int i;
   if (array == NULL) {
        return;
    }
    
    for ( i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    
    free(array);
}

char **read_stdin(char *input) {
    char **argz = malloc(sizeof(char*) * MAX_ARZ);
    char *toksign;
    int v = 0;
int i = 0 ;

    if (argz == NULL) {
        return NULL;
    }

    toksign = strtok(input, " ");
    
    for (v = 0; toksign != NULL; v++) {
        if (v >= MAX_ARZ - 1) {
            
            for (i = 0; i < v; i++) {
                free(argz[i]);
            }
            free(argz);
            return NULL;
        }

        argz[v] = malloc(sizeof(char) * MAXA_LEN);
        if (argz[v] == NULL) {
             
            for (i = 0; i < v; i++) {
                free(argz[i]);
            }
            free(argz);
            return NULL;
        }

        strcpy(argz[v], toksign);

        toksign = strtok(NULL, " ");
    }

    argz[v] = NULL;

    return argz;
}

char* find_wayy(char* commands) {
    char *wayy = getenv("PATH");
    char *dr;
    char *fulls_canal = malloc(sizeof(char) * (MAXC_LEN + 1));

    if (fulls_canal == 0) {
        return (NULL);
    }

    dr = strtok(wayy, ":");
    while (dr != 0) {
        snprintf(fulls_canal, MAXC_LEN, "%s/%s", dr, commands);

        if (access(fulls_canal, X_OK) == 0) {
            return fulls_canal;
        }

        dr = strtok(NULL, ":");
    }

    free(fulls_canal);

    return (NULL);
}

void exes_cmds(char** args) {
    char *cmd_path;
    pid_t pds = fork();
    if (pds == 0) {
        if (args[0][0] == '/') {
            execve(args[0], args, NULL);
        } else {
           cmd_path = find_wayy(args[0]);

            if (cmd_path == 0) {
                printf("%s: No such file or directory\n",  exe_name_cmd[0]);
                exit(1);
            }

            execve(cmd_path, args, NULL);
free (cmd_path);

        }
        printf("%s: failed to execute\n", args[0]);
        exit(1);
    } else if (pds > 0) {
        wait(NULL);
    } else {
        exit(1);
    }
}

void exit_myshell(char** args) {
    if (strcmp(args[0], "exit") == 0) {
        if (args[1] != NULL) {

            int exit_status = atoi(args[1]);
free_char_array(args);
            exit(exit_status);
        } else {
free_char_array(args);

            exit(0); 
        }
    }
}

void change_directory(char **args) {
    char *new_directory;
    char *previous_directory = current_directory;
    
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        new_directory = getenv("HOME");
    } else if (strcmp(args[1], "-") == 0) {
        new_directory = getenv("OLDPWD");
    } else {
        new_directory = args[1];
    }

    if (chdir(new_directory) == 0) {
        setenv("OLDPWD", current_directory, 1);
        current_directory = getcwd(NULL
, 0);
setenv("PWD", current_directory, 1);
} else {
printf("cd: %s: No such file or directory\n", new_directory);
}
if (previous_directory != NULL)
    free(previous_directory);
}

void printing_env() {
extern char **environ;
char **env = environ;
while (*env) {
printf("%s\n", *env);
env++;
}
}

ssize_t _getly(char **lineptr, size_t *n, FILE *stream) {
ssize_t num_chars = 0;
ssize_t bufsize = 0;
int c;
if (lineptr == NULL || n == NULL || stream == NULL) {
    return -1;
}


if (*lineptr == NULL || *n == 0) {
    *lineptr = malloc(128);
    if (*lineptr == NULL) {
        return -1;
    }
    *n = 128;
}

while ((c = fgetc(stream)) != '\n' && c != EOF) {
    (*lineptr)[num_chars] = c;
    num_chars++;


    if ((size_t) num_chars >= *n) {
        bufsize = *n + 128;
        *lineptr = realloc(*lineptr, bufsize);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = bufsize;
    }
}


(*lineptr)[num_chars] = '\0';

free(lineptr);
return num_chars;
}

int main(__attribute__((unused))int argc, char* argv[]) {
char *stdput = NULL;
size_t stdput_leng = 0;
char **args;
exe_name_cmd = argv;
current_directory = getcwd(NULL, 0);
setenv("PWD", current_directory, 1);

while (1) {
    printf("$ ");
    _getly(&stdput, &stdput_leng, stdin);
    stdput[strcspn(stdput, "\n")] = 0;

    args = read_stdin(stdput);

    if (args[0] != NULL) {
        if (strcmp(args[0], "exit") == 0) {
            exit_myshell(args);
        } else if (strcmp(args[0], "env") == 0) {
            printing_env();
        } else if (strcmp(args[0], "cd") == 0) {
            change_directory(args);
        } else {
            exes_cmds(args);
        }
    }
}

free(stdput);
free(current_directory);
return 0;
}
