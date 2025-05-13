CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = minishell
SRC = src/main.c src/builtins.c src/execute.c src/redirect.c src/pipe.c src/env.c src/history.c src/tokenizer.c
LIBS = -lreadline

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	rm -f $(OBJ) $(TARGET)
