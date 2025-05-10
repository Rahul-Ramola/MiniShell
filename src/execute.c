// execute.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("🚫 MiniShell");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("❌ fork failed");
    }
}
