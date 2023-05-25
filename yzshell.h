#ifndef SHELL_H
#define SHELL_H

extern char **environ;
extern char **zak;

void run_mycode(void);
void exes_cmdd(char **args);
void com_execve(char **args);
char **pars_args(char *linP);
int fun_bult(char **args);
void exec_bultts(char **args);
void free_argsss(char **args);
void print_out(void);
void print_myid(void);
void print_cid(void);
void printstt(char *F_name);
char *_getenv(const char *name);
char *get_ly(void);

#endif /* SHELL_H */

