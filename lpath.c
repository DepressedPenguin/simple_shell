char* find_wayy(char* commands) {
    char *wayy = getenv("PATH");
    char *dr;
    char *fulls_canal = malloc(sizeof(char) * (MAXC_LEN + 1));

    if (fulls_canal == 0) {
        return (NULL);
    }

    dr = strtok(wayy, ":");
    while (dr != 0) {
        snprintf(fulls_canal, MAXC_LEN, "%s/%s", dr, commands);

        if (access(fulls_canal, X_OK) == 0) {
            return fulls_canal;
        }

        dr = strtok(NULL, ":");
    }

    free(fulls_canal);

    return (NULL);
}

