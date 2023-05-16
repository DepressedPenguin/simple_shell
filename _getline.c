#include "myshell.h"
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

    if ((size_t)num_chars >= *n) {
        bufsize = *n + 128;
        *lineptr = realloc(*lineptr, bufsize);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = bufsize;
    }
}

(*lineptr)[num_chars] = '\0';

return num_chars;
}


