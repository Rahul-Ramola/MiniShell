#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/tokenizer.h"

void tokenize_input(const char *input, char *args[MAX_ARGS]) {
    int i = 0;
    int in_quote = 0;
    char buffer[1024];
    int buf_index = 0;

    for (int j = 0; input[j] != '\0'; j++) {
        char c = input[j];

        // Start or end quote
        if (c == '"') {
            in_quote = !in_quote;
            continue;
        }

        // If space and not in quote, end token
        if (isspace(c) && !in_quote) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                args[i++] = strdup(buffer);
                buf_index = 0;
            }
        } else {
            buffer[buf_index++] = c;
        }
    }

    // Final token
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        args[i++] = strdup(buffer);
    }

    args[i] = NULL;
}
