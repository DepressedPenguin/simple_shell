#include "myshell.h"

void exes_cmds(char** args) {
    pid_t pds = fork();
    if (pds == 0) {
        if (args[0][0] == '/') {
            execve(args[0], args, NULL);
        } else {
            char *cmd_path = find_wayy(args[0]);

            if (cmd_path == 0) {
                printf("%s: No such file or directory\n",  exe_name_cmd[0]);
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
