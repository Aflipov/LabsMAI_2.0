#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Создание списка
void list_create(List* list) {
    list->head = NULL;
    list->size = 0;
}

// Добавление в конец списка
void list_add_end(List* list, key_type key, value_type value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Ошибка: не удалось выделить память.\n");
        return;
    }
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++; // Увеличиваем размер списка
}

// Удаление списка
void list_delete(List* list) {
    Node* current = list->head;
    Node* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    list->head = NULL;
    list->size = 0;
}

// Печать списка
void list_print(const List* list) {
    if (list->head == NULL) {
        printf("Список пуст.\n");
        return;
    }
    Node* current = list->head;
    printf("Содержимое списка:\n");
    size_t index = 0;  // Инициализируем индекс
    while (current != NULL) {
        printf("Индекс: %zu, Ключ: %d, Значение: %s\n", index, current->key, current->value); // Выводим индекс
        current = current->next;
        index++; // Увеличиваем индекс для следующего элемента
    }
}

// Вернуть элемент по индексу
Node* list_get_by_index(const List* list, size_t index) {
    if (index >= list->size) {
        printf("Ошибка: Индекс за пределами списка.\n");
        return NULL;
    }
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

// Найти предыдущий элемент
Node* list_find_previous(const List* list, Node* node) {
    if (list->head == NULL || list->head == node) {
        return NULL;
    }
    Node* current = list->head;
    while (current->next != NULL && current->next != node) {
        current = current->next;
    }
    if (current->next == node) {
        return current;
    } else {
        return NULL; // Node не найден
    }
}

// Вставить элемент перед указанным узлом
void list_insert_before(List* list, Node* before, key_type key, value_type value) {
    if (before == NULL) {
        printf("Ошибка: Указанный узел не существует.\n");
        return;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Ошибка: не удалось выделить память.\n");
        return;
    }
    newNode->key = key;
    strcpy(newNode->value, value);

    if (list->head == before) { // Вставка в начало списка
        newNode->next = list->head;
        list->head = newNode;
    } else {
        Node* previous = list_find_previous(list, before);
        if (previous == NULL) {
            printf("Ошибка: Указанный узел не найден в списке.\n");
            free(newNode);
            return;
        }
        newNode->next = before;
        previous->next = newNode;
    }
    list->size++;
}

// Удалить узел по индексу
void list_delete_by_index(List* list, size_t index) {
    if (index >= list->size) {
        printf("Ошибка: Индекс за пределами списка.\n");
        return;
    }

    Node* nodeToDelete;

    if (index == 0) { // Удаление первого элемента
        nodeToDelete = list->head;
        list->head = nodeToDelete->next;
    } else {
        Node* previous = list_get_by_index(list, index - 1);
        nodeToDelete = previous->next;
        previous->next = nodeToDelete->next;
    }

    free(nodeToDelete);
    list->size--;
}

// Получение размера списка
size_t list_size(const List* list) {
    return list->size;
}

// Проверка, пуст ли список
bool list_is_empty(const List* list) {
    return list->head == NULL;
}