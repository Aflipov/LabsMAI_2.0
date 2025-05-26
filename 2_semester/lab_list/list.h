#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int key_type;
typedef char value_type[50];

typedef struct Node {
    key_type key;
    value_type value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    size_t size; // Добавим поле для хранения размера списка
} List;

// Прототипы функций
void list_create(List* list);
void list_add_end(List* list, key_type key, value_type value);
void list_delete(List* list);
void list_print(const List* list);
Node* list_get_by_index(const List* list, size_t index);
Node* list_find_previous(const List* list, Node* node);
void list_insert_before(List* list, Node* before, key_type key, value_type value);
void list_delete_by_index(List* list, size_t index);
size_t list_size(const List* list); // Добавлено для получения размера списка
bool list_is_empty(const List* list); // Добавлено для проверки, пуст ли список

#endif