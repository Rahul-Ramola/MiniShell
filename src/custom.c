#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // for chdir, close, access
#include <dirent.h>     // for DIR, opendir, readdir, closedir
#include <fcntl.h>      // for open, O_CREAT, O_WRONLY
#include <sys/stat.h>   // for stat, chmod, mkdir
#include <ctype.h>      // for isspace
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
        printf("  cd, pwd, echo, export, unset, history, exit, mkdir, cat, sort, ls, lsall, touch, rm, rmdir, chmod  \n");
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

    if (strcmp(args[0], "credit") == 0) {
        printf("⚡ MiniShell: A lightweight command line interface  \n");
        printf("💡 MiniShell by Code Masters\n");
        printf("📅 Developed for PBL(OS) Project\n");
        printf("👨‍💻 Contributors: Rahul Ramola, Krishna Sori, Priyanshu Chauhan, Ansh Sharma\n");
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

    // 1. ls
    if (strcmp(args[0], "ls") == 0) {
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (dir->d_name[0] != '.')
                    printf("%s  ", dir->d_name);
            }
            printf("\n");
            closedir(d);
        } else perror("ls");
        return 1;
    }

    // 2. lsall
    if (strcmp(args[0], "lsall") == 0) {
        DIR *d = opendir(".");
        struct dirent *dir;
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                printf("%s  ", dir->d_name);
            }
            printf("\n");
            closedir(d);
        } else perror("lsall");
        return 1;
    }

    // 3. sortfile <filename>
    if (strcmp(args[0], "sortfile") == 0) {
        if (!args[1]) {
            printf("Usage: sortfile <filename>\n");
            return 1;
        }
        FILE *fp = fopen(args[1], "r");
        if (!fp) {
            perror("sortfile");
            return 1;
        }
        char *lines[1000];
        char buffer[1024];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), fp)) {
            lines[count++] = strdup(buffer);
        }
        fclose(fp);
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(lines[i], lines[j]) > 0) {
                    char *tmp = lines[i];
                    lines[i] = lines[j];
                    lines[j] = tmp;
                }
            }
        }
        for (int i = 0; i < count; i++) {
            printf("%s", lines[i]);
            free(lines[i]);
        }
        return 1;
    }

    // 4. count <filename>
    if (strcmp(args[0], "count") == 0) {
        if (!args[1]) {
            printf("Usage: count <filename>\n");
            return 1;
        }
        FILE *fp = fopen(args[1], "r");
        if (!fp) {
            perror("count");
            return 1;
        }
        int ch, lines = 0, words = 0, chars = 0;
        int in_word = 0;
        while ((ch = fgetc(fp)) != EOF) {
            chars++;
            if (ch == '\n') lines++;
            if (isspace(ch)) in_word = 0;
            else if (!in_word) { in_word = 1; words++; }
        }
        fclose(fp);
        printf("Lines: %d\nWords: %d\nCharacters: %d\n", lines, words, chars);
        return 1;
    }

    // 5. touch <filename>
    if (strcmp(args[0], "touch") == 0) {
        if (!args[1]) {
            printf("Usage: touch <filename>\n");
            return 1;
        }
        int fd = open(args[1], O_CREAT | O_WRONLY, 0644);
        if (fd < 0) perror("touch");
        else close(fd);
        return 1;
    }

    // 6. rm <filename>
    if (strcmp(args[0], "rm") == 0) {
        if (!args[1]) {
            printf("Usage: rm <filename>\n");
            return 1;
        }
        if (remove(args[1]) != 0) perror("rm");
        return 1;
    }

    // 7. mkdir <dirname>
    if (strcmp(args[0], "mkdir") == 0) {
        if (!args[1]) {
            printf("Usage: mkdir <dirname>\n");
            return 1;
        }
        if (mkdir(args[1], 0755) != 0) perror("mkdir");
        return 1;
    }

    // 8. rmdir <dirname>
    if (strcmp(args[0], "rmdir") == 0) {
        if (!args[1]) {
            printf("Usage: rmdir <dirname>\n");
            return 1;
        }
        if (rmdir(args[1]) != 0) perror("rmdir");
        return 1;
    }

    // 9. chmod <mode> <file>
    if (strcmp(args[0], "chmod") == 0) {
        if (!args[1] || !args[2]) {
            printf("Usage: chmod <mode> <file>\n");
            return 1;
        }
        mode_t mode = strtol(args[1], 0, 8);
        if (chmod(args[2], mode) != 0) perror("chmod");
        return 1;
    }

    // 10. stat <filename>
    if (strcmp(args[0], "stat") == 0) {
        if (!args[1]) {
            printf("Usage: stat <filename>\n");
            return 1;
        }
        struct stat st;
        if (stat(args[1], &st) == 0) {
            printf("Size: %ld bytes\n", st.st_size);
            printf("Permissions: %o\n", st.st_mode & 0777);
            printf("Last modified: %ld\n", st.st_mtime);
        } else perror("stat");
        return 1;
    }

    if (strcmp(args[0], "sort") == 0) {
        char *lines[1000];
        char buffer[1024];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), stdin)) {
            if (strcmp(buffer, "\n") == 0) break;
            lines[count++] = strdup(buffer);
        }
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(lines[i], lines[j]) > 0) {
                    char *tmp = lines[i];
                    lines[i] = lines[j];
                    lines[j] = tmp;
                }
            }
        }
        for (int i = 0; i < count; i++) {
            printf("%s", lines[i]);
            free(lines[i]);
        }
        return 1;
    }

    if (strcmp(args[0], "cat") == 0) {
        if (!args[1]) {
            printf("Usage: cat <filename>\n");
            return 1;
        }
        FILE *fp = fopen(args[1], "r");
        if (!fp) {
            perror("cat");
            return 1;
        }
        char ch;
        while ((ch = fgetc(fp)) != EOF) {
            putchar(ch);
        }
        fclose(fp);
        return 1;
    }

    return 0;  // not a custom command
}
