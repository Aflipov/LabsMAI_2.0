#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "sort.h"

int main() {
    stack myStack;
    stack_create(&myStack);

    // Добавляем элементы в стек
    data_type item1 = {5, "Five"};
    data_type item2 = {2, "Two"};
    data_type item3 = {8, "Eight"};
    data_type item4 = {1, "One"};
    data_type item5 = {9, "Nine"};

    stack_push(&myStack, item1);
    stack_push(&myStack, item2);
    stack_push(&myStack, item3);
    stack_push(&myStack, item4);
    stack_push(&myStack, item5);

    printf("Исходный стек:\n");
    stack_print(&myStack);

    // Сортируем стек
    stack_sort_selection(&myStack);

    printf("\nОтсортированный стек:\n");
    stack_print(&myStack);

    return 0;
}