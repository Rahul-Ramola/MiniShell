CC = gcc
CFLAGS = -Wall -Wextra -Werror -D__USE_MINGW_ANSI_STDIO=1 -mconsole
SRC = src/main.c src/builtins.c src/execute.c src/redirect.c src/pipe.c src/signals.c src/env.c src/history.c
OBJ = $(SRC:.c=.o)
TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
