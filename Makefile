CC = gcc                     # Compiler
CFLAGS = -Wall -Wextra -Werror  # Compiler flags (warn about errors)
SRC = src/main.c src/builtins.c src/execute.c src/redirect.c src/pipe.c src/signals.c src/env.c src/history.c
OBJ = $(SRC:.c=.o)            # Object files (compiled .o files)
TARGET = minishell            # Output executable file

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
