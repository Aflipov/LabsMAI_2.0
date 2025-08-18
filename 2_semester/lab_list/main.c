#include <stdio.h>
#include "list.h"

void menu() {
    printf("\n===== Односвязный список =====\n");
    printf("1. Добавить элемент в конец\n");
    printf("2. Вставить элемент по индексу\n");
    printf("3. Удалить элемент по индексу\n");
    printf("4. Получить элемент по индексу\n");
    printf("5. Печать списка\n");
    printf("6. Очистить список\n");
    printf("0. Выход\n");
    printf("Выберите пункт: ");
}

int main() {
    List list;
    initList(&list);

    int choice, value, index;
    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите значение: ");
                scanf("%d", &value);
                pushBack(&list, value);
                break;

            case 2:
                printf("Введите индекс: ");
                scanf("%d", &index);
                printf("Введите значение: ");
                scanf("%d", &value);
                insertAt(&list, index, value);
                break;

            case 3:
                printf("Введите индекс: ");
                scanf("%d", &index);
                removeAt(&list, index);
                break;

            case 4:
                printf("Введите индекс: ");
                scanf("%d", &index);
                if (index >= 0 && index < list.size) {
                    printf("Элемент [%d] = %d\n", index, getAt(&list, index));
                } else {
                    printf("Ошибка: индекс вне диапазона!\n");
                }
                break;

            case 5:
                printList(&list);
                break;

            case 6:
                clearList(&list);
                printf("Список очищен.\n");
                break;

            case 0:
                clearList(&list);
                printf("Выход...\n");
                return 0;

            default:
                printf("Неверный выбор!\n");
        }
    }
}
