#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct
{
    int *data;
    int top;
    int capacity;
} Stack;

Stack *create_stack(int capacity);
void free_stack(Stack *s);

bool is_empty(Stack *s);
bool is_full(Stack *s);

bool push(Stack *s, int value);
int pop(Stack *s);
int peek(Stack *s);

void print_stack(Stack *s);

#endif
