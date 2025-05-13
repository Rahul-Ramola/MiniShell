// src/pipe.c
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/pipe.h"

// void execute_pipeline(char *commands[MAX_COMMANDS][MAX_ARGS], int num_cmds) {
//     int pipefd[2];
//     int prev_fd = -1;

//     for (int i = 0; i < num_cmds; i++) {
//         // Create pipe except after last command
//         if (i < num_cmds - 1) {
//             if (pipe(pipefd) == -1) {
//                 perror("❌ pipe");
//                 exit(EXIT_FAILURE);
//             }
//         }

//         pid_t pid = fork();
//         if (pid == 0) {
//             // CHILD PROCESS

//             if (prev_fd != -1) {
//                 dup2(prev_fd, STDIN_FILENO);  // Read from previous pipe
//                 close(prev_fd);
//             }

//             if (i < num_cmds - 1) {
//                 close(pipefd[0]);  // Close read end
//                 dup2(pipefd[1], STDOUT_FILENO);  // Write to pipe
//                 close(pipefd[1]);
//             }

//             execvp(commands[i][0], commands[i]);
//             perror("🚫 execvp failed");
//             exit(EXIT_FAILURE);
//         } else if (pid < 0) {
//             perror("❌ fork");
//             exit(EXIT_FAILURE);
//         }

//         // PARENT PROCESS
//         if (prev_fd != -1)
//             close(prev_fd);

//         if (i < num_cmds - 1) {
//             close(pipefd[1]);  // Close write end in parent
//             prev_fd = pipefd[0];  // Save read end for next command
//         }
//     }

//     // Wait for all children
//     for (int i = 0; i < num_cmds; i++)
//         wait(NULL);
// }

// Helper to detect and apply redirection on a command
// void handle_pipe_redirection(char **args, int position, int total) {
//     for (int i = 0; args[i] != NULL; i++) {
//         // OUTPUT
//         if ((strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) && position == total - 1) {
//             int fd;
//             if (strcmp(args[i], ">>") == 0)
//                 fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
//             else
//                 fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

//             if (fd < 0) {
//                 perror("🚫 Pipe redirect failed");
//                 exit(EXIT_FAILURE);
//             }

//             dup2(fd, STDOUT_FILENO);
//             close(fd);

//             args[i] = NULL;        // ✅ Remove '>' or '>>'
//             args[i + 1] = NULL;    // ✅ Remove filename
//             break;                 // ✅ Stop scanning
//         }

//         // INPUT
//         if (strcmp(args[i], "<") == 0 && position == 0) {
//             int fd = open(args[i + 1], O_RDONLY);
//             if (fd < 0) {
//                 perror("🚫 Pipe input file");
//                 exit(EXIT_FAILURE);
//             }
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//             args[i] = NULL;
//             args[i + 1] = NULL;
//             break;
//         }
//     }

// }


void handle_pipe_redirection(char **args, int position, int total) {
    for (int i = 0; args[i] != NULL; i++) {
        // OUTPUT Redirection (last command only)
        if ((strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) && position == total - 1) {
            int fd;
            if (strcmp(args[i], ">>") == 0)
                fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0) {
                perror("🚫 Pipe output redirection failed");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);

            // Remove > and filename by shifting left
            for (int j = i; args[j + 2] != NULL; j++) {
                args[j] = args[j + 2];
            }
            args[strlen(args[i])] = NULL;
            break;
        }

        // INPUT Redirection (first command only)
        if (strcmp(args[i], "<") == 0 && position == 0) {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("🚫 Pipe input redirection failed");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);

            // Remove < and filename by shifting
            for (int j = i; args[j + 2] != NULL; j++) {
                args[j] = args[j + 2];
            }
            args[strlen(args[i])] = NULL;
            break;
        }
    }
}

void execute_pipeline(char *commands[MAX_COMMANDS][MAX_ARGS], int num_cmds) {
    int pipefd[2];
    int prev_fd = -1;

    for (int i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1) {
            if (pipe(pipefd) == -1) {
                perror("❌ pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            // CHILD
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < num_cmds - 1) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            handle_pipe_redirection(commands[i], i, num_cmds);  // ✅ redirection per segment

            execvp(commands[i][0], commands[i]);
            perror("🚫 exec failed");
            exit(EXIT_FAILURE);
        }

        // PARENT
        if (prev_fd != -1)
            close(prev_fd);
        if (i < num_cmds - 1) {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
    }

    for (int i = 0; i < num_cmds; i++)
        wait(NULL);
}

// Returns 1 if there's a pipe symbol
int is_pipeline(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            return 1;
        }
    }
    return 0;
}

// Splits args into multiple command segments by '|'
int split_pipeline(char **args, char *commands[MAX_COMMANDS][MAX_ARGS]) {
    int cmd_index = 0, arg_index = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            commands[cmd_index][arg_index] = NULL;  // null-terminate this command
            cmd_index++;
            arg_index = 0;
        } else {
            commands[cmd_index][arg_index++] = args[i];
        }
    }

    commands[cmd_index][arg_index] = NULL;  // null-terminate final command
    return cmd_index + 1;  // number of commands
}
