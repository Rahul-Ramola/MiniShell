// include/pipe.h
#ifndef PIPE_H
#define PIPE_H

#define MAX_COMMANDS 10
#define MAX_ARGS 100

void execute_pipeline(char *commands[MAX_COMMANDS][MAX_ARGS], int num_cmds);
int is_pipeline(char **args);
int split_pipeline(char **args, char *commands[MAX_COMMANDS][MAX_ARGS]);

#endif
