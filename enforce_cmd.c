#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void enforce_cmd(char** arjs) {
    pid_t pids = fork();
    if (pids == 0) {

        char *envp[] = { NULL };
        execve(arjs[0], arjs, envp);
        printf("No such file or directory\n");
        exit(1);
    } else if (pids > 0){

        wait(NULL);
    } else {
      
        printf("Failed to fork process\n");
        exit(1);
    }
}
