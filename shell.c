#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *btn = NULL;
    size_t stdinz_size = 0;
    char **arjs;

    while (1) {
        printf("$ ");
        getline(&btn, &stdinz_size, stdin);

        btn[strlen(btn)-1] = 0;

        arjs = read_stdins(btn);

        enforce_cmd(arjs);
    }

    free(btn);
    return 0;
}

