#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

// Создание стека
void stack_create(stack *s) {
    s->top = -1; // Пустой стек
}

// Проверка, пуст ли стек
bool stack_is_empty(const stack *s) {
    return s->top == -1;
}

// Проверка, полон ли стек
bool stack_is_full(const stack *s) {
    return s->top == STACK_SIZE - 1;
}

// Добавление элемента в стек
void stack_push(stack *s, data_type data) {
    if (stack_is_full(s)) {
        printf("Ошибка: Стек полон!\n");
        return;
    }
    s->top++;
    s->items[s->top] = data;
}

// Удаление элемента из стека
data_type stack_pop(stack *s) {
    data_type temp;
    if (stack_is_empty(s)) {
        printf("Ошибка: Стек пуст!\n");
        temp.key = -1; // Признак ошибки
        strcpy(temp.value, "ERROR"); // Признак ошибки
        return temp;
    }
    temp = s->items[s->top];
    s->top--;
    return temp;
}

// Печать стека
void stack_print(const stack *s) {
    if (stack_is_empty(s)) {
        printf("Стек пуст.\n");
        return;
    }
    printf("Содержимое стека:\n");
    for (int i = s->top; i >= 0; i--) {
        printf("Ключ: %d, Значение: %s\n", s->items[i].key, s->items[i].value);
    }
}

// Получение размера стека
size_t stack_size(const stack *s) {
    return s->top + 1;
}

// Поиск и удаление максимального элемента
data_type stack_delete_max(stack *s) {
    if (stack_is_empty(s)) {
        data_type temp;
        printf("Стек пуст.\n");
        temp.key = -1;
        strcpy(temp.value, "ERROR");
        return temp;
    }
    int max_index = 0;
    for (int i = 1; i <= s->top; ++i) {
        if (s->items[i].key > s->items[max_index].key) {
            max_index = i;
        }
    }

    data_type max_value = s->items[max_index];

    //Сдвигаем элементы после max_index на одну позицию влево
    for (int i = max_index; i < s->top; ++i) {
        s->items[i] = s->items[i + 1];
    }

    s->top--; // Уменьшаем размер стека

    return max_value;

}

//Функция поиска максимального элемента в стеке
data_type stack_find_max(stack *s) {
    data_type temp;
    if (stack_is_empty(s)) {
        printf("Стек пуст.\n");
        temp.key = -1;
        strcpy(temp.value, "ERROR");
        return temp;
    }

    int max_index = 0; // Индекс максимального элемента
    for (int i = 1; i <= s->top; i++) {
        if (s->items[i].key > s->items[max_index].key) {
            max_index = i;
        }
    }
    return s->items[max_index];
}