#include "myshell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include "myshell.h"


void enforce_cmd(char** arjs) {
    pid_t pids = fork();
    if (pids == 0) {

        char *envp[] = { NULL };
        execve(arjs[0], arjs, envp);
        printf("./shell: No such file or directory\n");
        exit(1);
    } else if (pids > 0){

        wait(NULL);
    } else {
      
        printf("Failed to fork process\n");
        exit(1);
    }
}
