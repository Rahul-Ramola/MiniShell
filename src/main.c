#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <fcntl.h>    

#include <readline/readline.h>
#include <readline/history.h>


#include "builtins.h"
#include "execute.h"
#include "redirect.h"
#include "pipe.h"
#include "env.h"
#include "history.h"
#include "tokenizer.h"
#include "custom.h"

#define MAX_INPUT 1024
#define MAX_ARGS 100

void shell_loop() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    
    while (1) {
        // printf("🐚 MiniShell> ");
        // fflush(stdout);

        // if (fgets(input, MAX_INPUT, stdin) == NULL) {
        //     printf("\n👋 Exiting MiniShell...\n");
        //     break;
        // }

        // input[strcspn(input, "\n")] = 0;  // Trim newline
        // add_to_history(input);

        char *line = readline("🐚 MiniShell> ");
        if (!line) {
            printf("\n👋 Exiting MiniShell...\n");
            break;
        }

        if (strlen(line) > 0) {
            add_history(line);         // readline history
            add_to_history(line);      // your internal history
        }

        strncpy(input, line, MAX_INPUT);
        free(line);


        tokenize_input(input, args);

        expand_variables(args);

        if (args[0] == NULL) continue;

        if (is_pipeline(args)) {
            char *commands[MAX_COMMANDS][MAX_ARGS] = {0};
            int num_cmds = split_pipeline(args, commands);
            execute_pipeline(commands, num_cmds);
            continue;
        }


        if (handle_redirection(args) != 0) {
            restore_redirection();  // 🔧 always restore even after failure
            continue;
        }                 // 🔁 May modify STDOUT

        if (strcmp(args[0], "history") == 0) {
            show_history();
            restore_redirection();
            continue;
        }


        if (handle_builtin(args)) {
            restore_redirection();
            continue;
        }

        if (handle_env_builtin(args)) {
            restore_redirection(); // still needed after any command
            continue;
        }

        if (handle_custom_builtin(args)) {
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
