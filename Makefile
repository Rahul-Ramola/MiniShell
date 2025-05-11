CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = minishell
SRC = src/main.c src/builtins.c src/execute.c src/redirect.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
