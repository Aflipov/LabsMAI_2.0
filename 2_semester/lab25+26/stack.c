#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *create_stack(int capacity)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->data = (int *)malloc(sizeof(int) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void free_stack(Stack *s)
{
    free(s->data);
    free(s);
}

bool is_empty(Stack *s)
{
    return s->top == -1;
}

bool is_full(Stack *s)
{
    return s->top == s->capacity - 1;
}

bool push(Stack *s, int value)
{
    if (is_full(s))
    {
        printf("Ошибка: стек переполнен (%d/%d)\n", s->top + 1, s->capacity);
        return false;
    }
    s->data[++s->top] = value;
    return true;
}

int pop(Stack *s)
{
    if (is_empty(s))
    {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return s->data[s->top--];
}

int peek(Stack *s)
{
    if (is_empty(s))
    {
        fprintf(stderr, "Stack is empty\n");
        exit(1);
    }
    return s->data[s->top];
}

void print_stack(Stack *s)
{
    if (is_empty(s))
    {
        printf("Стек пуст\n");
        return;
    }

    printf("\nТекущее состояние стека (заполнено %d/%d):\n", s->top + 1, s->capacity);
    printf("↓ top\n");
    printf("-----------\n");

    for (int i = s->top; i >= 0; i--)
    {
        printf("| %3d |  (index %d)\n", s->data[i], i);
    }

    printf("-----------\n");
    // printf("↑ bottom\n");
}
