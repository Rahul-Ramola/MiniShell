#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "execute.h"

#define MAX_INPUT 1024
#define MAX_ARGS 100

void shell_loop() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("🐚 MiniShell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n👋 Exiting MiniShell...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;

        if (handle_builtin(args)) {
            continue;  // already executed
        }

        execute_command(args);  // external command
    }
}

int main() {
    printf("🎉 Welcome to MiniShell!\n");
    shell_loop();
    return 0;
}
