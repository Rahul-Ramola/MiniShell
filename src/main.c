#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <fcntl.h>    


#include "builtins.h"
#include "execute.h"
#include "redirect.h"

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

    if (handle_redirection(args) != 0) {
        restore_redirection();  // 🔧 always restore even after failure
        continue;
    }                 // 🔁 May modify STDOUT

        if (handle_builtin(args)) {
            restore_redirection();
            continue;
        }

        execute_command(args);

        restore_redirection();


        
    }
}

int main() {
    printf("🎉 Welcome to MiniShell!\n");
    shell_loop();
    return 0;
}
