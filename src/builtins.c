// builtins.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int handle_builtin(char **args) {
    if (args[0] == NULL) return 0;

    if (strcmp(args[0], "exit") == 0) {
        printf("👋 Goodbye!\n");
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "⚠️ MiniShell: expected argument to \"cd\"\n");
        } else if (chdir(args[1]) != 0) {
            perror("🚫 MiniShell");
        } else {
            printf("📁 Changed directory to %s\n", args[1]);
        }
        return 1;
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("📍 Current Directory: %s\n", cwd);
        } else {
            perror("❌ MiniShell");
        }
        return 1;
    } else if (strcmp(args[0], "echo") == 0) {
        printf("🗣️ ");
        for (int j = 1; args[j] != NULL; j++) {
            printf("%s ", args[j]);
        }
        printf("\n");
        return 1;
    }

    return 0; // Not a built-in
}
