#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int size;
} List;

// Создание/очистка
void initList(List* list);
void clearList(List* list);

// Базовые операции
void pushBack(List* list, int value);      // Добавление в конец
void printList(const List* list);          // Печать
int  getAt(const List* list, int index);   // Вернуть элемент по индексу

// Вставка / удаление
Node* findPrev(List* list, int index);     // Найти предыдущий элемент
void insertAt(List* list, int index, int value); // Вставить по индексу
void removeAt(List* list, int index);      // Удалить по индексу

#endif
