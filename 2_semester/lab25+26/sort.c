#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "sort.h" // Добавлен sort.h

// Сортировка стека линейным выбором
void stack_sort_selection(stack *s) {
    stack tempStack;
    stack_create(&tempStack);

    while (!stack_is_empty(s)) {
        data_type max_val = stack_delete_max(s);
        stack_push(&tempStack, max_val);
    }

    // Перемещаем элементы обратно в исходный стек
    while (!stack_is_empty(&tempStack)) {
        data_type val = stack_pop(&tempStack);
        stack_push(s, val);
    }
}