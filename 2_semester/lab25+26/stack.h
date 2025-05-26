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
void stack_push_front(stack *s, data_type data); // Вставить в начало (push)
void stack_push_back(stack *s, data_type data);  // Вставить в конец (push)
data_type stack_pop_front(stack *s);           // Удалить первый (pop)
data_type stack_pop_back(stack *s);            // Удалить последний (pop)
void stack_print(const stack *s);
size_t stack_size(const stack *s);
data_type stack_find_max(stack *s);      // Функция для поиска максимального элемента
data_type stack_delete_max(stack *s);    // Функция для поиска и удаления максимального элемента
void stack_sort_selection(stack *s);
void stack_concatenate(stack *s1, stack *s2);  // Конкатенация двух стеков
void stack_create_random(stack* s, int size);  // Создать случайный стек

#endif