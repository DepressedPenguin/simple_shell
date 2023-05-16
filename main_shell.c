#include "myshell.h"
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
