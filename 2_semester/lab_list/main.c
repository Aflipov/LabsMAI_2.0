#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main() {
    List myList;
    list_create(&myList);

    int choice, key, index;
    char value[50];
    Node* node;

    do {
        printf("\nМеню управления списком:\n");
        printf("1. Добавить в конец списка\n");
        printf("2. Удалить список\n");
        printf("3. Печать списка\n");
        printf("4. Вернуть элемент по индексу\n");
        printf("5. Найти предыдущий элемент\n");
        printf("6. Вставить элемент перед\n");
        printf("7. Удалить узел по индексу\n");
        printf("0. Выйти\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите ключ: ");
                scanf("%d", &key);
                printf("Введите значение: ");
                scanf("%s", value);
                list_add_end(&myList, key, value);
                printf("Элемент добавлен в конец списка.\n");
                break;
            case 2:
                list_delete(&myList);
                printf("Список удален.\n");
                break;
            case 3:
                list_print(&myList);
                break;
            case 4:
                printf("Введите индекс элемента: ");
                scanf("%d", &index);
                node = list_get_by_index(&myList, index);
                if (node != NULL) {
                    printf("Элемент с индексом %d: Ключ: %d, Значение: %s\n", index, node->key, node->value);
                }
                break;
            case 5:
                printf("Введите индекс элемента, для которого нужно найти предыдущий: ");
                scanf("%d", &index);
                node = list_get_by_index(&myList, index);
                if (node != NULL) {
                    Node* previous = list_find_previous(&myList, node);
                    if (previous != NULL) {
                        printf("Предыдущий элемент: Ключ: %d, Значение: %s\n", previous->key, previous->value);
                    } else {
                        printf("Нет предыдущего элемента.\n");
                    }
                }
                break;
            case 6:
                printf("Введите индекс элемента, ПЕРЕД которым нужно вставить новый элемент: ");
                scanf("%d", &index);
                Node* beforeNode = list_get_by_index(&myList, index);
                if (beforeNode != NULL) {
                    printf("Введите ключ нового элемента: ");
                    scanf("%d", &key);
                    printf("Введите значение нового элемента: ");
                    scanf("%s", value);
                    list_insert_before(&myList, beforeNode, key, value);
                    printf("Элемент вставлен перед указанным элементом.\n");
                }
                break;
            case 7:
                printf("Введите индекс элемента для удаления: ");
                scanf("%d", &index);
                list_delete_by_index(&myList, index);
                printf("Элемент с индексом %d удален.\n", index);
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