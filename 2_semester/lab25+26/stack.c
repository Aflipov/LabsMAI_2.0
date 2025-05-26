#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

// Вставка в начало (стека - push)
void stack_push_front(stack *s, data_type data) {
    if (stack_is_full(s)) {
        printf("Ошибка: Стек полон!\n");
        return;
    }
    s->top++;
    s->items[s->top] = data;
}

// Вставка в конец (стека - push) - то же самое, что и push_front
void stack_push_back(stack *s, data_type data) {
    stack_push_front(s, data);
}

// Удаление с начала (стека - pop)
data_type stack_pop_front(stack *s) {
    data_type temp = {0, ""}; // Инициализация нулями
    if (stack_is_empty(s)) {
        printf("Ошибка: Стек пуст!\n");
        temp.key = -1;
        strcpy(temp.value, "ERROR");
        return temp;
    }
    temp = s->items[s->top];
    s->top--;
    return temp;
}

// Удаление с конца (стека - pop) - то же самое, что и pop_front
data_type stack_pop_back(stack *s) {
    return stack_pop_front(s);
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
    data_type temp = {0, ""};
    if (stack_is_empty(s)) {
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
    data_type temp = {0, ""};
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

// Конкатенация двух стеков (s2 добавляется в конец s1)
void stack_concatenate(stack *s1, stack *s2) {
    while (!stack_is_empty(s2)) {
        data_type data = stack_pop_front(s2); // Извлекаем из s2
        if (!stack_is_full(s1)) { // Проверяем, что s1 не полон
            stack_push_front(s1, data); // Добавляем в s1
        } else {
            printf("Ошибка: Стек s1 полон, конкатенация прервана.\n");
            return;
        }
    }
}

// Создание случайного стека
void stack_create_random(stack* s, int size) {
    stack_create(s); // Инициализируем стек

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    for (int i = 0; i < size; ++i) {
        data_type item;
        item.key = rand() % 100; // Случайный ключ от 0 до 99
        sprintf(item.value, "Random %d", i); // Случайное значение
        stack_push_front(s, item);
    }
}