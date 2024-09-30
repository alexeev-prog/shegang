#ifndef EXECUTOR_H
#define EXECUTOR_H

int help(char** args);
int quit_from_shell(char** args);
int print_history(char** args);
int reload_shell(char** args);
int execute(char** args);

#endif // EXECUTOR_H
