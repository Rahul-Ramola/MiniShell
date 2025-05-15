#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/custom.h"
#include "builtins.h"
#include "execute.h"

int handle_custom_builtin(char **args) {
    if (args[0] == NULL) return 0;

    if (strcmp(args[0], "hello") == 0) {
        printf("👋 Hello from MiniShell! Have a great day.\n");
        return 1;
    }

    if (strcmp(args[0], "help") == 0) {
        printf("📖 MiniShell Commands:\n");
        printf("  cd, pwd, echo, export, unset, history, exit\n");
        printf("  hello, help, clr, calc, greet, time, version, repeat, files, dirs\n");
        return 1;
    }

    if (strcmp(args[0], "clr") == 0) {
        system("clear");
        return 1;
    }

    if (strcmp(args[0], "calc") == 0) {
        if (args[1] && args[2] && args[3]) {
            int a = atoi(args[1]);
            int b = atoi(args[3]);
            char op = args[2][0];
            int result = 0;
            if (op == '+') result = a + b;
            else if (op == '-') result = a - b;
            else if (op == '*') result = a * b;
            else if (op == '/') result = (b != 0) ? a / b : 0;
            else {
                printf("❌ Invalid operator\n");
                return 1;
            }
            printf("🧮 Result: %d\n", result);
        } else {
            printf("⚠️ Usage: calc 5 + 3\n");
        }
        return 1;
    }

    if (strcmp(args[0], "greet") == 0) {
        if (args[1]) {
            printf("👋 Hello, %s!\n", args[1]);
        } else {
            printf("👋 Hello, stranger!\n");
        }
        return 1;
    }

    if (strcmp(args[0], "time") == 0) {
        system("date");
        return 1;
    }

    if (strcmp(args[0], "version") == 0) {
        printf("🛠 MiniShell version 1.0 - May 2025\n");
        return 1;
    }

    if (strcmp(args[0], "repeat") == 0) {
        if (args[1] && args[2]) {
            int times = atoi(args[1]);
            for (int i = 0; i < times; i++) {
                char *cmd[] = {args[2], args[3], args[4], NULL}; // supports up to 2 args
                if (!handle_builtin(cmd))
                    execute_command(cmd);
            }
        } else {
            printf("⚠️ Usage: repeat 3 echo Hi\n");
        }
        return 1;
    }

    if (strcmp(args[0], "files") == 0) {
        system("find . -maxdepth 1 -type f");
        return 1;
    }

    if (strcmp(args[0], "dirs") == 0) {
        system("find . -maxdepth 1 -type d");
        return 1;
    }

    return 0;  // not a custom command
}
