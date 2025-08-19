#ifndef EXPR_H
#define EXPR_H

typedef struct Node {
    char* value;
    struct Node* left;
    struct Node* right;
} Node;

// Создание узлов дерева
Node* create_node(const char* value, Node* left, Node* right);

// Освобождение памяти
void free_tree(Node* root);

// Упрощение дробей
Node* simplify_tree(Node* root);

// Печать дерева в инфиксной форме
void print_infix(Node* root);

#endif
