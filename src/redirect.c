// src/redirect.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int saved_stdout = -1;
int saved_stdin = -1;

int handle_redirection(char **args) {
    int in_redirect = 0, out_redirect = 0, append_redirect = 0;
    char *input_file = NULL, *output_file = NULL;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0 && args[i + 1] != NULL) {
            in_redirect = 1;
            input_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0 && args[i + 1] != NULL) {
            append_redirect = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        } else if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL) {
            out_redirect = 1;
            output_file = args[i + 1];
            args[i] = NULL;
        }
    }

    // Save original stdout and stdin
    if (saved_stdout == -1)
        saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1)
        saved_stdin = dup(STDIN_FILENO);

    // Input redirection
    if (in_redirect && input_file) {
        int fd = open(input_file, O_RDONLY);
        if (fd < 0) {
            perror("🚫 Input redirection failed");
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // Output redirection
    if (out_redirect && output_file) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("🚫 Output redirection failed");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // Append redirection
    if (append_redirect && output_file) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror("🚫 Append redirection failed");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    return 0;
}

void restore_redirection() {
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        saved_stdout = -1;
    }

    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
        saved_stdin = -1;
    }
}

