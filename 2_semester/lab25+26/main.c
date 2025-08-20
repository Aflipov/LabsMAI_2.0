#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "sort.h"

int main()
{
    int capacity;
    printf("Введите размер стека: ");
    scanf("%d", &capacity);

    Stack *s = create_stack(capacity);

    int choice, value;
    while (1)
    {
        printf("\nМеню:\n");
        printf("1. Добавить элемент (push)\n");
        printf("2. Извлечь элемент (pop)\n");
        printf("3. Показать стек\n");
        printf("4. Сортировать стек (по возрастанию)\n");
        printf("5. Сортировать стек (по убыванию)\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Введите число: ");
            scanf("%d", &value);
            push(s, value);
            print_stack(s);
            break;
        case 2:
            if (!is_empty(s))
            {
                printf("\nИзвлечено: %d\n", pop(s));
                print_stack(s);
            }
            else
            {
                printf("Стек пуст!\n");
            }
            break;
        case 3:
            print_stack(s);
            break;
        case 4:
            if (!is_empty(s))
            {
                selection_sort_stack(s, 1);
                printf("Стек отсортирован по возрастанию.\n");
                print_stack(s);
            }
            else
            {
                printf("Стек пуст!\n");
            }
            break;
        case 5:
            if (!is_empty(s))
            {
                selection_sort_stack(s, -1);
                printf("Стек отсортирован по убыванию.\n");
                print_stack(s);
            }
            else
            {
                printf("Стек пуст!\n");
            }
            break;
        case 0:
            free_stack(s);
            printf("Выход...\n");
            return 0;
        default:
            printf("Неверный выбор!\n");
        }
    }
}
