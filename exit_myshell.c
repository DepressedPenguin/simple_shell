void exit_myshell() {
    printf("Exiting shell...\n");
    exit(0);
}

void printing_env() {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}
