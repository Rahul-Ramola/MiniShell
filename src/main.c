#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024  // Maximum command length

void shell_loop() {
    char input[MAX_INPUT];

    while (1) {  // Infinite loop
        printf("MiniShell> ");  // Display prompt
        fflush(stdout);  // Ensure prompt is printed immediately

        if (fgets(input, MAX_INPUT, stdin) == NULL) {  // Read input
            printf("\nExiting MiniShell...\n");
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Check for "exit" command
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        printf("You entered: %s\n", input);
    }
}

int main() {
    printf("Welcome to MiniShell!\n");
    shell_loop();
    return 0;
}
