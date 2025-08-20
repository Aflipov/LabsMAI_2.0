# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -MMD -MP

# Имя таргета
TARGET = expression_simplifier

# Исходники, объектные файлы и зависимости
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

# Определение OS
ifeq ($(OS),Windows_NT)
    EXE = .exe
    START = CMD /C START 
    RM = del /Q
    CLEAN = 2>nul
else
    EXE =
    START = ./
    RM = rm -f
    CLEAN = 
endif

# Цель по умолчанию
.SILENT:
all: $(TARGET)$(EXE)

# Сборка таргета
$(TARGET)$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Запуск
run: $(TARGET)$(EXE)
	$(START)$(TARGET)$(EXE)

# Очистка
clean:
	$(RM) $(TARGET)$(EXE) $(OBJ) $(DEP) $(CLEAN)

# Подключение зависимостей
-include $(DEP)
