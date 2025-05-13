#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/history.h"

#define MAX_HISTORY 100

static char *history[MAX_HISTORY];
static int history_count = 0;

void add_to_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    }
}

void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d  %s\n", i + 1, history[i]);
    }
}
