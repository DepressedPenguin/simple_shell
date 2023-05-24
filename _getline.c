#include "myshell.h"

/**
 * _getly - a line is read from a file stream.
 * @lineptr: the buffer where the line is saved, indicated by a pointer
 * @n: pointer to the buffer's size
 * @stream: stream of files to read from
 *
 * Return: number of characters read, -1 on failure
 */
ssize_t _getly(char **lineptr, size_t *n, FILE *stream)
{
ssize_t num_chars = 0;
ssize_t bufsize = 0;
int c;

        /* check for invalid parameters */
if (lineptr == NULL || n == NULL || stream == NULL)
return (-1);

        /* allocate buffer if not already allocated */
if (*lineptr == NULL || *n == 0)
{
*lineptr = malloc(128);
if (*lineptr == NULL)
return (-1);
*n = 128;
}

        /* read characters until end of line or end of file */
while ((c = fgetc(stream)) != '\n' && c != EOF)
{
                /* store character in buffer */
(*lineptr)[num_chars] = c;
num_chars++;

                /* resize buffer if necessary */
if ((size_t)num_chars >= *n)
{
bufsize = *n + 128;
*lineptr = realloc(*lineptr, bufsize);
if (*lineptr == NULL)
return (-1);
*n = bufsize;
}
}

        /* terminate string with null character */
(*lineptr)[num_chars] = '\0';

return (num_chars);
}
