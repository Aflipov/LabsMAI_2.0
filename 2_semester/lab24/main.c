#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 20

// Структура для узла дерева
typedef struct TreeNode {
    char data[MAX_TOKEN_LENGTH]; // Для операторов (+, *, /) и операндов (число или переменная)
    struct TreeNode *left; // Указатель на левого потомка узла
    struct TreeNode *right; // Указатель на правого потомка узла
} TreeNode;

// Функция для создания нового узла дерева
TreeNode *createNode(char *data) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->data, data); // Копирует данные в поле нового узла
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Функция для проверки, является ли строка оператором
int isOperator(char *str) {
    return (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ||
            strcmp(str, "*") == 0 || strcmp(str, "/") == 0 || strcmp(str, "u-") == 0);
}

// Функция для получения приоритета оператора для правильной расстановки операторов в постфиксной записи
int getPriority(char *oper) {
    if (strcmp(oper, "+") == 0 || strcmp(oper, "-") == 0) {
        return 1;
    } else if (strcmp(oper, "*") == 0 || strcmp(oper, "/") == 0) {
        return 2;
    } else if(strcmp(oper, "u-") == 0) { // Унарный минус
        return 3; // Более высокий приоритет
    }
    return 0; // Для операндов и переменных
}

// Функция для преобразования выражения в постфиксную запись (алгоритм сортировочной станции)
char** infixToPostfix(char *infix, int *postfixSize) {
    // ... (Код как в предыдущем примере, без изменений в этой части) ...
     // Выделяем память для массива строк, который будет содержать постфиксную запись
    // Предполагаем не более MAX_TOKENS токенов
    char** postfix = (char**)malloc(MAX_TOKENS * sizeof(char*));
    if (postfix == NULL) {
        perror("Ошибка выделения памяти для postfix");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_TOKENS; ++i) {
        postfix[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        if (postfix[i] == NULL) {
            perror("Ошибка выделения памяти для postfix[i]");
            exit(EXIT_FAILURE);
        }
    }

    // Выделяет память для стека
    char** stack = (char**)malloc(MAX_TOKENS * sizeof(char*));
    if (stack == NULL) {
        perror("Ошибка выделения памяти для stack");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_TOKENS; ++i) {
        stack[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        if (stack[i] == NULL) {
            perror("Ошибка выделения памяти для stack[i]");
            exit(EXIT_FAILURE);
        }
    }

    int stackTop = -1; // Индекс вершины стека (пустой стек)
    int postfixIndex = 0; // Индекс для добавления токенов в постфиксную запись
    char *token = strtok(infix, " "); // Разделяем строку на токены по пробелам
    // strtok - изменяет сроку, поэтому важно передавать копию строки

    bool expectOperand = true; // Ожидаем операнд (число, переменную или открывающую скобку)

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // Если токен - число (или отрицательное)
            strcpy(postfix[postfixIndex++], token);
            expectOperand = false;
        } else if (isalpha(token[0])) { // Добавлена проверка на переменную
            // Если токен - переменная (начинается с буквы)
            strcpy(postfix[postfixIndex++], token);
            expectOperand = false;
        } else if (strcmp(token, "(") == 0) {
            strcpy(stack[++stackTop], token);
            expectOperand = true; // После открывающей скобки ожидаем операнд
        } else if (strcmp(token, ")") == 0) {
             while (stackTop >= 0 && strcmp(stack[stackTop], "(") != 0) {
                strcpy(postfix[postfixIndex++], stack[stackTop--]);
            }
            if (stackTop >= 0 && strcmp(stack[stackTop], "(") == 0) {
                stackTop--; // Удаляем открывающую скобку
            } else {
                fprintf(stderr, "Ошибка: Непарные скобки\n");
                 for (int i = 0; i < MAX_TOKENS; ++i) {
                    free(stack[i]);
                }
                free(stack);
                for (int i = 0; i < MAX_TOKENS; ++i) {
                    free(postfix[i]);
                }
                free(postfix);
                exit(EXIT_FAILURE);
            }
             expectOperand = false;
        } else if (isOperator(token)) {
            if (expectOperand && strcmp(token, "-") == 0) {
                 strcpy(stack[++stackTop], "u-");
            } else {
                 while (stackTop >= 0 && strcmp(stack[stackTop], "(") != 0 && getPriority(stack[stackTop]) >= getPriority(token)) {
                    strcpy(postfix[postfixIndex++], stack[stackTop--]);
                }
                strcpy(stack[++stackTop], token);
            }
            expectOperand = true; // После оператора ожидаем операнд

        } else {
             fprintf(stderr, "Ошибка: Неверный символ %s\n", token);
            for (int i = 0; i < MAX_TOKENS; ++i) {
                free(stack[i]);
            }
            free(stack);
            for (int i = 0; i < MAX_TOKENS; ++i) {
                free(postfix[i]);
            }
            free(postfix);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " "); // Получаем следующий токен
    }

     while (stackTop >= 0) {
        if (strcmp(stack[stackTop], "(") == 0) {
            fprintf(stderr, "Ошибка: Непарные скобки\n");
             for (int i = 0; i < MAX_TOKENS; ++i) {
                free(stack[i]);
            }
            free(stack);
            for (int i = 0; i < MAX_TOKENS; ++i) {
                free(postfix[i]);
            }
            free(postfix);
            exit(EXIT_FAILURE);
        }
        strcpy(postfix[postfixIndex++], stack[stackTop--]);
    }

    *postfixSize = postfixIndex; //Размер постфиксной записи

    for (int i = 0; i < MAX_TOKENS; ++i) {
        free(stack[i]);
    }
    free(stack);

    return postfix;
}

// Функция для построения дерева выражения из постфиксной записи
TreeNode *buildExpressionTree(char** postfix, int postfixSize) {
    TreeNode **stack = (TreeNode **)malloc(postfixSize * sizeof(TreeNode *));
    if (stack == NULL) {
        perror("Ошибка выделения памяти для стека дерева");
        exit(EXIT_FAILURE);
    }

    int stackTop = -1; // Индекс вершины стека

    for (int i = 0; i < postfixSize; i++) {
        if (!isOperator(postfix[i])) { // Если операнд
            stack[++stackTop] = createNode(postfix[i]); // Создается новый узел дерева и добавляется в стек
        } else {
            TreeNode *newNode = createNode(postfix[i]); // Создается новый узел дерева
            if (stackTop < 1) {
               fprintf(stderr, "Ошибка: Неверное количество операндов для оператора %s\n", postfix[i]);
               free(stack); // Освобождаем память перед выходом
               return NULL; // Возвращаем NULL, чтобы показать ошибку
             }

            // Из стека извлекаются два последних узла
            newNode->right = stack[stackTop--];
            newNode->left = stack[stackTop--];

            // Новый узел помещается в стек
            stack[++stackTop] = newNode;
        }
    }

    TreeNode *root = (stackTop == 0) ? stack[stackTop] : NULL; // Проверяем, что в стеке остался только корень

    if (stackTop != 0) {
       fprintf(stderr, "Ошибка: Неверное выражение в постфиксной записи\n");
       free(stack);
       return NULL;
    }

    free(stack); //// Освобождаем память стека
    return root;
}

// Функция для освобождения памяти, выделенной под дерево
void freeTree(TreeNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Вспомогательная функция для обхода дерева и сбора токенов (используется в simplifyFraction)
static void traverseTree(TreeNode* node, char** tokens, int* count) {
    if (node == NULL) return;

    if (isOperator(node->data) && strcmp(node->data, "*") == 0) {
        traverseTree(node->left, tokens, count);
        traverseTree(node->right, tokens, count);
    } else {
        strcpy(tokens[(*count)++], node->data);
    }
}

// Вспомогательная функция для построения дерева из токенов после сокращения (используется в simplifyFraction)
static TreeNode* buildSubTree(char** tokens, int count) {
    TreeNode* subtree = NULL;
    if (count > 0) {
        // Создаем дерево, перемножая токены
        subtree = createNode(tokens[0]);
        for (int i = 1; i < count; i++) {
            TreeNode* mulNode = createNode("*");
            mulNode->left = subtree;
            mulNode->right = createNode(tokens[i]);
            subtree = mulNode;
        }
    }
    return subtree;
}

// Функция для упрощения дроби, сокращая общие члены
TreeNode* simplifyFraction(TreeNode* root) {
    if (root == NULL || strcmp(root->data, "/") != 0) {
        return root; // Упрощать нужно только корень, являющийся делением
    }

    TreeNode* numerator = root->left; //числитель
    TreeNode* denominator = root->right; //знаменатель

    // Выделяем память под массивы для хранения токенов числителя и знаменателя
    char** numTokens = (char**)malloc(MAX_TOKENS * sizeof(char*));
    char** denTokens = (char**)malloc(MAX_TOKENS * sizeof(char*));

    for (int i = 0; i < MAX_TOKENS; i++) {
        numTokens[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        denTokens[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        if (!numTokens[i] || !denTokens[i]) {
            fprintf(stderr, "Ошибка: недостаточно памяти\n");
              for (int j = 0; j < i; j++) {
                  free(numTokens[j]);
                  free(denTokens[j]);
              }
            free(numTokens);
            free(denTokens);

            return root;
        }
    }

    int numCount = 0;
    int denCount = 0;

    // Обходим дерево для числителя и знаменателя и сохраняем токены
    // Для упрощения предполагаем, что числитель и знаменатель содержат только *
    traverseTree(numerator, numTokens, &numCount);
    traverseTree(denominator, denTokens, &denCount);

    // Сокращаем общие токены
    for (int i = 0; i < numCount; i++) {
        if (strcmp(numTokens[i], "USED") == 0) continue;
        for (int j = 0; j < denCount; j++) {
            if (strcmp(denTokens[j], "USED") == 0) continue;
            if (strcmp(numTokens[i], denTokens[j]) == 0) {
                strcpy(numTokens[i], "USED");
                strcpy(denTokens[j], "USED");
                break;
            }
        }
    }

    // Создаем массивы токенов, пропуская "USED"
    char** newNumTokens = (char**)malloc(MAX_TOKENS * sizeof(char*));
    char** newDenTokens = (char**)malloc(MAX_TOKENS * sizeof(char*));
    for (int i = 0; i < MAX_TOKENS; i++) {
        newNumTokens[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        newDenTokens[i] = (char*)malloc(MAX_TOKEN_LENGTH * sizeof(char));
        if (!newNumTokens[i] || !newDenTokens[i]) {
             fprintf(stderr, "Ошибка: недостаточно памяти\n");
             for (int j = 0; j < i; j++) {
                  free(newNumTokens[j]);
                  free(newDenTokens[j]);
              }
            free(newNumTokens);
            free(newDenTokens);
            return root;
        }
    }

    int newNumCount = 0;
    int newDenCount = 0;

     for (int i = 0; i < numCount; i++) {
        if (strcmp(numTokens[i], "USED") != 0) {
            strcpy(newNumTokens[newNumCount++], numTokens[i]);
        }
    }

    for (int i = 0; i < denCount; i++) {
        if (strcmp(denTokens[i], "USED") != 0) {
            strcpy(newDenTokens[newDenCount++], denTokens[i]);
        }
    }

    // Строим новые деревья для числителя и знаменателя
    TreeNode* newNumerator = buildSubTree(newNumTokens, newNumCount);
    TreeNode* newDenominator = buildSubTree(newDenTokens, newDenCount);

    // Освобождаем старые деревья
    freeTree(numerator);
    freeTree(denominator);

    // Освобождаем память
     for (int i = 0; i < MAX_TOKENS; i++) {
            free(numTokens[i]);
            free(denTokens[i]);
     }

      for (int i = 0; i < MAX_TOKENS; i++) {
            free(newNumTokens[i]);
            free(newDenTokens[i]);
     }

    free(numTokens);
    free(denTokens);
    free(newNumTokens);
    free(newDenTokens);

    // Обновляем числитель и знаменатель в корневом узле
    root->left = newNumerator;
    root->right = newDenominator;

    return root;
}

// Функция для преобразования a / b + c / d -> (a * d + b * c)/(b * d)
// TreeNode *transformTree(TreeNode *root) {
//     if (root == NULL) {
//         return NULL;
//     }

//     if (strcmp(root->data, "+") == 0 && root->left != NULL && strcmp(root->left->data, "/") == 0 && root->right != NULL && strcmp(root->right->data, "/") == 0) {
//         TreeNode *a = root->left->left;
//         TreeNode *b = root->left->right;
//         TreeNode *c = root->right->left;
//         TreeNode *d = root->right->right;

//         // Создаем новые узлы для (a * d + b * c)/(b * d)
//         TreeNode *mul1 = createNode("*");
//         mul1->left = createNode(a->data);
//         mul1->right = createNode(d->data);

//         TreeNode *mul2 = createNode("*");
//         mul2->left = createNode(b->data);
//         mul2->right = createNode(c->data);

//         TreeNode *add = createNode("+");
//         add->left = mul1;
//         add->right = mul2;

//         TreeNode *mul3 = createNode("*");
//         mul3->left = createNode(b->data);
//         mul3->right = createNode(d->data);

//         TreeNode *div = createNode("/");
//         div->left = add;
//         div->right = mul3;

//         // Освобождаем память от старых узлов и их потомков
//         free(root->left->left);
//         free(root->left->right);
//         free(root->left);
//         free(root->right->left);
//         free(root->right->right);
//         free(root->right);
//         free(root);

//        return div; // Возвращаем корен преобразованного поддерева
//     } else {
//          TreeNode* newRoot = createNode(root->data);
//         newRoot->left = transformTree(root->left);
//         newRoot->right = transformTree(root->right);
//         // Освобождаем память от старого root
//         free(root->left);
//         free(root->right);
//         free(root);

//         return newRoot;
//     }
// }

// Функция для печати дерева в инфиксной записи
void printInfix(TreeNode *root) {
    if (root != NULL) {
         if (strcmp(root->data, "u-") == 0) {
            printf("-"); // Особый случай для унарного минуса
            printInfix(root->right);
        } else {
            if (isOperator(root->data) && root->left && root->right) {
                printf("(");
            }
            printInfix(root->left);
            printf(" %s ", root->data);
            printInfix(root->right);
            if (isOperator(root->data) && root->left && root->right) {
                printf(")");
            }
        }
    }
}

// Функция для печати дерева в виде дерева (с отступами)
void printTreeIndented(TreeNode *root, int level) {
   if (root == NULL) {
        return;
    }

    // Увеличиваем отступ для каждого уровня
    for (int i = 0; i < level; i++) {
        printf("    "); // 4 пробела для каждого уровня
    }

     if (strcmp(root->data, "u-") == 0) {
            printf("u-\n");
        } else {
             printf("%s\n", root->data);
        }

    // Рекурсивно печатаем правое и левое поддеревья
    printTreeIndented(root->left, level + 1);
    printTreeIndented(root->right, level + 1);
}

// Вспомогательная функция для вызова из main
void printTreeWrapper(TreeNode *root) {
    printTreeIndented(root, 0);
}

int main() {
    char expression[MAX_TOKENS * 2];

    printf("Введите арифметическое выражение (например: a * b * 3 / 3 * b * c): ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = 0; // Удаляем символ новой строки, который добавляет fgets

    // Преобразуем введенное выражение в дерево
    int postfixSize;
    char* infixCopy = strdup(expression);  // Копируем строку, чтобы strtok не изменил исходную строку
    if(infixCopy == NULL){
      perror("Ошибка выделения памяти для infixCopy");
      exit(EXIT_FAILURE);
    }

    char** postfix = infixToPostfix(infixCopy, &postfixSize);
    free(infixCopy);

    TreeNode *root = buildExpressionTree(postfix, postfixSize);

    if(root == NULL) {
        // Ошибка при построении дерева, нужно освободить выделенную память и выйти
        for (int i = 0; i < MAX_TOKENS; ++i) {
            free(postfix[i]);
        }
        free(postfix);
        return 1; // Или другой код ошибки
    }

    for (int i = 0; i < MAX_TOKENS; ++i) {
        free(postfix[i]);
    }
    free(postfix);


    printf("Исходное выражение (дерево): \n");
    printTreeWrapper(root);
    printf("\n");

    printf("Исходное выражение (текст): ");
    printInfix(root);
    printf("\n");

    if (strcmp(root->data, "/") == 0) { // Упрощаем, только если выражение - дробь
        root = simplifyFraction(root);
    }

    printf("Упрощенное выражение (дерево): \n");
    printTreeWrapper(root);
    printf("\n");

    printf("Упрощенное выражение (текст): ");
    printInfix(root);
    printf("\n");

    // Освобождаем память, выделенную под дерево
    freeTree(root); // Используем root, т.к. transformTree больше не нужна

    return 0;
}