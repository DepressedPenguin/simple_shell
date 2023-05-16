#include "myshell.h"

void exit_myshell(char** args) {
    if (strcmp(args[0], "exit") == 0) {
    if (args[1] != NULL) {
        int exit_status = atoi(args[1]);
        exit(exit_status);
    } else {
        exit(0);
    }
}


}

void printing_env(void) {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}
