#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Структура узла двоичного дерева
typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Функция создания нового узла дерева
TreeNode* createNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Функция сравнения двух узлов дерева (для типа char)
int compareNodes(const void *a, const void *b) {
    return (*(char*)a - *(char*)b);
}

// Функция печати узла дерева (для типа char)
void printNode(const void *data, int level) {
    for (int i = 0; i < level; i++) {
        printf("  "); // Отступы для визуализации глубины
    }
    printf("%c\n", *(char*)data);
}

// Добавление нового узла (для двоичного дерева с сохранением порядка)
TreeNode* insertNode(TreeNode* root, char data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (compareNodes(&data, &(root->data)) < 0) {
        root->left = insertNode(root->left, data);
    } else {
        root->right = insertNode(root->right, data);
    }

    return root;
}

// Текстовая визуализация дерева (значение каждого узла выводится в отдельной строке, с отступом)
void printTree(TreeNode* root, int level) {
    if (root != NULL) {
        printNode(&(root->data), level);  // Печатаем текущий узел (сначала)
        printTree(root->left, level + 1);   // Затем левое поддерево
        printTree(root->right, level + 1);  // Затем правое поддерево
    }
}

// Удаление узла (двоичное дерево перестраивается в соответствии с требованием сохранения целостности и порядка)
TreeNode* deleteNode(TreeNode* root, char data) {
    if (root == NULL) {
        return root;
    }

    if (compareNodes(&data, &(root->data)) < 0) {
        root->left = deleteNode(root->left, data);
    } else if (compareNodes(&data, &(root->data)) > 0) {
        root->right = deleteNode(root->right, data);
    } else {
        // Нашли узел для удаления
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Узел с двумя потомками: найти наименьший элемент в правом поддереве
        TreeNode* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        // Скопировать содержимое "преемника" в этот узел
        root->data = temp->data;

        // Удалить "преемника"
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

// Вычисление числа вершин двоичного дерева
int countNodes(TreeNode* root) {
    if (root == NULL) {
        return 0;
    } else {
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
}

// Функция освобождения памяти, занимаемой деревом
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void runMenu(TreeNode** root) {  // Принимаем указатель на указатель
    char choice;

    do {
        printf("\nМеню:\n");
        printf("1. Добавить узел\n");
        printf("2. Вывести дерево\n");
        printf("3. Удалить узел\n");
        printf("4. Подсчитать число вершин\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1': {
                char data;
                printf("Введите данные узла (char): ");
                scanf(" %c", &data);
                *root = insertNode(*root, data); // Используем * для доступа к значению по указателю
                break;
            }
            case '2':
                printf("Дерево:\n");
                printTree(*root, 0);
                break;
            case '3': {
                char data;
                printf("Введите данные узла для удаления (char): ");
                scanf(" %c", &data);
                *root = deleteNode(*root, data);
                break;
            }
            case '4':
                printf("Число вершин: %d\n", countNodes(*root));
                break;
            case '0':
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор.\n");
        }
    } while (choice != '0');
}

int main() {
    TreeNode* root = NULL;

    runMenu(&root);

    freeTree(root);
    return 0;
}