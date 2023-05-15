int main(__attribute__((unused))int argc, char* argv[]) {
    char *stdput = NULL;
    size_t stdput_leng = 0;
    char **args;

    exe_name_cmd = argv;

    while (1) {
        printf("$ ");
        getline(&stdput, &stdput_leng, stdin);
        stdput[strcspn(stdput, "\n")] = 0;

        args = read_stdin(stdput);

        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                exit_myshell();
            } else if (strcmp(args[0], "env") == 0) {
                printing_env();
            } else {
                exes_cmds(args);
            }
        }
    }

    free(stdput);
    return 0;
}
