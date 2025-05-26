#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "sort.h"

int main() {
    stack myStack;
    stack_create(&myStack);

    int choice, size, key;
    data_type item;
    stack anotherStack; // Для конкатенации

    do {
        printf("\nМеню управления стеком:\n");
        printf("1. Создать случайный стек\n");
        printf("3. Вывести размер стека\n");
        printf("4. Вставить в начало стека\n");
        printf("5. Вставить в конец стека\n");
        printf("6. Удалить первый элемент стека\n");
        printf("7. Удалить последний элемент стека\n");
        printf("8. Сортировка стека\n");
        printf("10. Конкатенация двух стеков\n");
        printf("0. Выйти\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите размер стека: ");
                scanf("%d", &size);
                stack_create_random(&myStack, size);
                printf("Стек создан.\n");
                break;
            case 3:
                printf("Размер стека: %zu\n", stack_size(&myStack));
                break;
            case 4:
                printf("Введите ключ элемента: ");
                scanf("%d", &key);
                item.key = key;
                printf("Введите значение элемента: ");
                scanf("%s", item.value);
                stack_push_front(&myStack, item);
                printf("Элемент добавлен в начало стека.\n");
                break;
            case 5:
                printf("Введите ключ элемента: ");
                scanf("%d", &key);
                item.key = key;
                printf("Введите значение элемента: ");
                scanf("%s", item.value);
                stack_push_back(&myStack, item);
                printf("Элемент добавлен в конец стека.\n");
                break;
            case 6:
                item = stack_pop_front(&myStack);
                if (item.key != -1) {
                    printf("Удален первый элемент: Ключ: %d, Значение: %s\n", item.key, item.value);
                }
                break;
            case 7:
                item = stack_pop_back(&myStack);
                if (item.key != -1) {
                    printf("Удален последний элемент: Ключ: %d, Значение: %s\n", item.key, item.value);
                }
                break;
            case 8:
                stack_sort_selection(&myStack);
                printf("Стек отсортирован.\n");
                break;
            case 10:
                stack_create(&anotherStack);
                printf("Создайте второй стек (введите размер): ");
                scanf("%d", &size);
                stack_create_random(&anotherStack, size);
                stack_concatenate(&myStack, &anotherStack);
                printf("Стеки объединены.\n");
                break;
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный ввод.\n");
        }
    } while (choice != 0);

    return 0;
}