# Makefile для сборки программы работы с линейным списком

# Имя компилятора
CC = gcc

# Имя редактора связей
LD = gcc

# Флаги компиляции
CCFLAGS = -g -Wall -pedantic -std=c99

# Флаги связывания
LDFLAGS =

# Имя исполняемого файла
TARGET = list_manager

# Список объектных файлов
OBJ = list.o main.o

# Правило по умолчанию: создать исполняемый файл
all: $(TARGET)

# Правило для создания исполняемого файла из объектных файлов
$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJ)

# Правила для создания объектных файлов из соответствующих исходных файлов
list.o: list.c list.h
	$(CC) $(CCFLAGS) -c list.c

main.o: main.c list.h
	$(CC) $(CCFLAGS) -c main.c

# Правило для очистки проекта
clean:
	rm -f $(OBJ) $(TARGET)

# Указание суффиксов
.SUFFIXES: .c .o