#include "myshell.h"
char* _mygitl(void) {
    static char buffer[BUFFER_SIZE];
    static size_t buffer_index = 0;
    static size_t buffer_size = 0;
    static int end_of_file = 0;
    

    char* line = NULL;
    size_t line_length = 0;
    

    while (1) {

        if (buffer_index >= buffer_size) {
            buffer_size = fread(buffer, sizeof(char), BUFFER_SIZE, stdin);
            buffer_index = 0;
            
            
            if (buffer_size == 0) {
                end_of_file = 1;
                break;
            }
        }
        

        while (buffer_index < buffer_size) {

            if (buffer[buffer_index] == '\n') {
                line = realloc(line, line_length + 1);
                line[line_length] = '\0';
                buffer_index++; 
                return (line);
            }
            

            line = realloc(line, line_length + 1);
            line[line_length] = buffer[buffer_index];
            line_length++;
            buffer_index++;
        }
    }
    

    if (end_of_file) {
        free(line);
        return (NULL);
    }
    
    return (line); 
}

