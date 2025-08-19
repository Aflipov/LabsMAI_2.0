#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expr.h"

// ---------- Вспомогательные функции ----------

Node* create_node(const char* value, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->value = strdup(value);
    n->left = left;
    n->right = right;
    return n;
}

void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->value);
    free(root);
}

int is_number(const char* s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// ---------- Сбор множителей ----------

typedef struct {
    Node** data;
    int size;
    int capacity;
} FactorList;

void list_init(FactorList* list) {
    list->size = 0;
    list->capacity = 8;
    list->data = malloc(list->capacity * sizeof(Node*));
}

void list_append(FactorList* list, Node* n) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(Node*));
    }
    list->data[list->size++] = n;
}

void collect_factors(Node* root, FactorList* list) {
    if (!root) return;
    if (strcmp(root->value, "*") == 0) {
        collect_factors(root->left, list);
        collect_factors(root->right, list);
    } else {
        list_append(list, root);
    }
}

// ---------- Построение дерева из множителей ----------

Node* build_tree_from_factors(FactorList* list) {
    if (list->size == 0) return NULL;
    if (list->size == 1) return list->data[0];

    Node* res = list->data[0];
    for (int i = 1; i < list->size; i++) {
        res = create_node("*", res, list->data[i]);
    }
    return res;
}

// ---------- Упрощение дроби ----------

Node* simplify_fraction(Node* node) {
    if (!node || strcmp(node->value, "/") != 0) return node;

    FactorList num, den;
    list_init(&num);
    list_init(&den);

    collect_factors(node->left, &num);
    collect_factors(node->right, &den);

    // --- Сокращение переменных ---
    for (int i = 0; i < num.size; i++) {
        for (int j = 0; j < den.size; j++) {
            if (num.data[i] && den.data[j]) {
                if (!is_number(num.data[i]->value) && !is_number(den.data[j]->value)) {
                    if (strcmp(num.data[i]->value, den.data[j]->value) == 0) {
                        free(num.data[i]->value);
                        num.data[i]->value = strdup("1");
                        free(den.data[j]->value);
                        den.data[j]->value = strdup("1");
                    }
                }
            }
        }
    }

    // --- Сокращение чисел через gcd ---
    for (int i = 0; i < num.size; i++) {
        if (num.data[i] && is_number(num.data[i]->value)) {
            int a = atoi(num.data[i]->value);
            for (int j = 0; j < den.size; j++) {
                if (den.data[j] && is_number(den.data[j]->value)) {
                    int b = atoi(den.data[j]->value);
                    int g = gcd(a, b);
                    if (g > 1) {
                        a /= g;
                        b /= g;
                        free(num.data[i]->value);
                        free(den.data[j]->value);
                        char buf1[32], buf2[32];
                        sprintf(buf1, "%d", a);
                        sprintf(buf2, "%d", b);
                        num.data[i]->value = strdup(buf1);
                        den.data[j]->value = strdup(buf2);
                    }
                }
            }
        }
    }

    // --- Удаление множителей == "1" ---
    FactorList clean_num, clean_den;
    list_init(&clean_num);
    list_init(&clean_den);

    for (int i = 0; i < num.size; i++) {
        if (num.data[i] && !(is_number(num.data[i]->value) && atoi(num.data[i]->value) == 1)) {
            list_append(&clean_num, num.data[i]);
        } else {
            free_tree(num.data[i]); // очищаем узел с "1"
        }
    }

    for (int j = 0; j < den.size; j++) {
        if (den.data[j] && !(is_number(den.data[j]->value) && atoi(den.data[j]->value) == 1)) {
            list_append(&clean_den, den.data[j]);
        } else {
            free_tree(den.data[j]);
        }
    }

    free(num.data);
    free(den.data);

    Node* new_num = build_tree_from_factors(&clean_num);
    Node* new_den = build_tree_from_factors(&clean_den);

    free(clean_num.data);
    free(clean_den.data);

    if (!new_num) new_num = create_node("1", NULL, NULL);
    if (!new_den) return new_num;
    return create_node("/", new_num, new_den);
}

// ---------- Упрощение дерева ----------

Node* simplify_tree(Node* root) {
    if (!root) return NULL;
    root->left = simplify_tree(root->left);
    root->right = simplify_tree(root->right);

    if (strcmp(root->value, "/") == 0) {
        return simplify_fraction(root);
    }
    return root;
}

// ---------- Печать в инфиксной форме ----------

void print_infix(Node* root) {
    if (!root) return;
    if (!root->left && !root->right) {
        printf("%s", root->value);
        return;
    }
    printf("(");
    print_infix(root->left);
    printf(" %s ", root->value);
    print_infix(root->right);
    printf(")");
}
