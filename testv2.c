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

char **read_stdin(char *input) {
    char **argz = malloc(sizeof(char*) * MAX_ARZ);
    char *toksign;
    int v = 0;

    if (argz == 0) {
        return (NULL);
    }

    toksign = strtok(input, " ");
    
    for ( v = 0 ; toksign != 0 ; v++) {
        if (v >= MAX_ARZ - 1) {
            return (NULL);
        }

        argz[v] = malloc(sizeof(char) * MAXA_LEN);
        if (argz[v] == 0) {
             return (NULL);
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
    pid_t pds = fork();
    if (pds == 0) {
        if (args[0][0] == '/') {
            execve(args[0], args, NULL);
        } else {
            char *cmd_path = find_wayy(args[0]);

            if (cmd_path == 0) {
                printf("%s: No such file or directory\n", args[0]);
                exit(1);
            }

            execve(cmd_path, args, NULL);
        }
        printf("%s: failed to execute\n", args[0]);
        exit(1);
    } else if (pds > 0) {
        wait(NULL);
    } else {
        exit(1);
    }
}


void exit_myshell() {
    printf("Exiting shell...\n");
    exit(0);
}

void printing_env() {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

int main(__attribute__((unused))int argc, char* argv[]) {
    char *stdput = NULL;
    size_t stdput_leng = 0;
    char **args;

    exe_name_cmd = argv;

    while (1) {
        printf("$ ");
        getline(&stdput, &stdput_leng, stdin);
        stdput[strcspn(stdput, "\n")] = 0;

        args = read_stdin(stdput);

        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                exit_myshell();
            } else if (strcmp(args[0], "env") == 0) {
                printing_env();
            } else {
                exes_cmds(args);
            }
        }
    }

    free(stdput);
    return 0;
}

