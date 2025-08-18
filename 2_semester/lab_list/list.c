#include "list.h"

void initList(List* list) {
    list->head = NULL;
    list->size = 0;
}

void clearList(List* list) {
    Node* curr = list->head;
    while (curr) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
    list->size = 0;
}

void pushBack(List* list, int value) {
    Node* newNode = malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
    } else {
        Node* curr = list->head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    list->size++;
}

void printList(const List* list) {
    Node* curr = list->head;
    while (curr) {
        printf("%d -> ", curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
}

int getAt(const List* list, int index) {
    if (index < 0 || index >= list->size) {
        printf("Ошибка: индекс вне диапазона!\n");
        exit(1);
    }
    Node* curr = list->head;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->value;
}

Node* findPrev(List* list, int index) {
    if (index <= 0 || index > list->size) return NULL;

    Node* curr = list->head;
    for (int i = 0; i < index - 1; i++) {
        curr = curr->next;
    }
    return curr;
}

void insertAt(List* list, int index, int value) {
    if (index < 0 || index > list->size) {
        printf("Ошибка: индекс вне диапазона!\n");
        return;
    }

    Node* newNode = malloc(sizeof(Node));
    newNode->value = value;

    if (index == 0) {
        newNode->next = list->head;
        list->head = newNode;
    } else {
        Node* prev = findPrev(list, index);
        newNode->next = prev->next;
        prev->next = newNode;
    }

    list->size++;
}

void removeAt(List* list, int index) {
    if (index < 0 || index >= list->size) {
        printf("Ошибка: индекс вне диапазона!\n");
        return;
    }

    Node* toDelete;
    if (index == 0) {
        toDelete = list->head;
        list->head = list->head->next;
    } else {
        Node* prev = findPrev(list, index);
        toDelete = prev->next;
        prev->next = toDelete->next;
    }
    free(toDelete);
    list->size--;
}
