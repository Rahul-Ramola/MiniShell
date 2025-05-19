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
        char cwd[1024];
        char *target_dir = args[1];

        // Save current directory before changing
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("🚫 MiniShell: getcwd");
            return 1;
        }

        if (target_dir == NULL) {
            // No argument: go to HOME
            target_dir = "/mnt/e/PBL(OS)/MiniShell";
            if (target_dir == NULL) {
                fprintf(stderr, "⚠️ MiniShell: HOME not set\n");
                return 1;
            }
        } else if (strcmp(target_dir, "-") == 0) {
            // Go to previous directory
            target_dir = getenv("OLDPWD");
            if (target_dir == NULL) {
                fprintf(stderr, "⚠️ MiniShell: OLDPWD not set\n");
                return 1;
            }
            printf("%s\n", target_dir);
        }

        if (chdir(target_dir) != 0) {
            perror("🚫 MiniShell");
        } else {
            // Update OLDPWD and PWD
            setenv("OLDPWD", cwd, 1);
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                setenv("PWD", cwd, 1);
            }
            printf("📁 Changed directory to %s\n", target_dir);
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
        if (isatty(STDOUT_FILENO)) {
            printf("🗣️ ");
        }
        for (int j = 1; args[j] != NULL; j++) {
            printf("%s ", args[j]);
        }
        printf("\n");
        return 1;
    }

    return 0; 
}
