#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Определите типы ключа и значения
typedef int key_type;   // Пример: ключ - целое число
typedef char value_type[50]; // Пример: значение - строка

typedef struct {
    key_type key;
    value_type value;
} data_type;

#define STACK_SIZE 100 // Максимальный размер стека

typedef struct {
    data_type items[STACK_SIZE];
    int top; // Индекс вершины стека
} stack;

// Прототипы функций
void stack_create(stack *s);
bool stack_is_empty(const stack *s);
bool stack_is_full(const stack *s); // Добавлено
void stack_push(stack *s, data_type data);
data_type stack_pop(stack *s);  // Изменено: возвращает значение
void stack_print(const stack *s);
size_t stack_size(const stack *s);
// void stack_insert(stack *s, data_type data); // Можете не реализовывать, если не требуется
// void stack_erase(stack *s, key_type key);   // Можете не реализовывать, если не требуется
data_type stack_find_max(stack *s);      // Функция для поиска максимального элемента
data_type stack_delete_max(stack *s);    // Функция для поиска и удаления максимального элемента

#endif