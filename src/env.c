#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void expand_variables(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *var_name = args[i] + 1;  // skip $
            char *value = getenv(var_name);
            if (value) {
                args[i] = value;  // Replace with value
            } else {
                args[i] = "";  // Undefined variable → empty
            }
        }
    }
}

int handle_env_builtin(char **args) {
    if (args[0] == NULL) return 0;

    if (strcmp(args[0], "export") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "⚠️ Usage: export VAR=value\n");
            return 1;
        }

        char *pair = args[1];
        char *equal = strchr(pair, '=');
        if (!equal) {
            fprintf(stderr, "⚠️ Invalid format. Use VAR=value\n");
            return 1;
        }

        *equal = '\0';  // split into key and value
        char *key = pair;
        char *value = equal + 1;

        if (setenv(key, value, 1) != 0) {
            perror("🚫 export failed");
        }
        return 1;
    }

    if (strcmp(args[0], "unset") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "⚠️ Usage: unset VAR\n");
            return 1;
        }

        if (unsetenv(args[1]) != 0) {
            perror("🚫 unset failed");
        }
        return 1;
    }

    return 0;  // not handled
}
