CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -MMD -MP
TARGET = expression_simplifier

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJ) $(TARGET) $(DEP)

-include $(DEP)
