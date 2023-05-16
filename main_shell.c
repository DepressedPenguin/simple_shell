#include "myshell.h"

int main(__attribute__((unused))int argc, char* argv[]) {
    char _stdput[BUFFER_SIZE];
    char **args;

    exe_name_cmd = argv;

    while (1) {
        printf("$ ");
	strcpy(_stdput , _mygitl());
        _stdput[strcspn(_stdput, "\n")] = 0;

        args = read_stdin(_stdput);

        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                exit_myshell(args);
            } else if (strcmp(args[0], "env") == 0) {
                printing_env();
            } else {
                exes_cmds(args);
            }
        }
    }

    free(_stdput);
    return 0;
}
