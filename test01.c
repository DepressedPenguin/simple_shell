#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXC_LENZ 1024
#define MAX_ARGZ 64
#define MAXA_LENZ 128

char* search_path(char* cmd);

char **read_stdins(char *input) {
    char **argz = (char **) malloc(sizeof(char*) * MAX_ARGZ);
    
    if (argz == 0) {
        return (NULL);
    }

    char *sign = strtok(input, " ");
    int x = 0;
    for (x = 0 ; sign != NULL ;x++) {
        if (x >= MAX_ARGZ - 1) {
        return (NULL);
        }

        argz[x] = (char*) malloc(sizeof(char) * MAXA_LENZ);
        if (argz[x] == 0) {
         return (NULL);
        }

        strcpy(argz[x], sign);

        sign = strtok(NULL, " ");
    }

    argz[x] = NULL;

    return (argz);
}

void enforce_cmd(char** arjs) {
    pid_t pids = fork();
    if (pids == 0) {
        // Child process
        execvp(arjs[0], arjs);
        printf(" ./shell: No such file or directory\n");
        exit(1);
    } else if (pids > 0) {
        // Parent process
        wait(NULL);
    } else {
        // Error
        printf("Failed to fork process\n");
        exit(1);
    }
}

char *search_path(char *cmd) {
    char *path_env = getenv("PATH");
    char *dir = strtok(path_env, ":");
    char *full_path = malloc(MAXC_LENZ);

    // First, check for command in /bin directory
    sprintf(full_path, "/bin/%s", cmd);
    if (access(full_path, F_OK | X_OK) == 0) {
        // Command found in /bin directory
        return full_path;
    }

    // If not found in /bin, search other directories in PATH
    while (dir != NULL) {
        sprintf(full_path, "%s/%s", dir, cmd);
        if (access(full_path, F_OK | X_OK) == 0) {
            // Command found in directory
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    // Command not found in PATH
    return NULL;
}

int main() {
    char *btn = NULL;
    size_t stdinz_size = 0;
    char **arjs;

    while (1) {
        printf("$ ");
        getline(&btn, &stdinz_size, stdin);

        btn[strlen(btn)-1] = 0;

        arjs = read_stdins(btn);

        char *full_path = search_path(arjs[0]);
        if (full_path != NULL) {
            // Command found in PATH, execute it
            arjs[0] = full_path;
            enforce_cmd(arjs);
            free(full_path);
        } else {
            // Command not found in PATH
            printf("Command not found: %s\n", arjs[0]);
        }

        free(arjs);
    }

    free(btn);
    return 0;
}

