#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Структура для узла дерева
typedef struct TreeNode {
    char data[20]; // Для операторов (+, *, /) и операндов (число или переменная)
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
            strcmp(str, "*") == 0 || strcmp(str, "/") == 0);
    //// strcmp - функция возвращает 1, если строка str и (+. -, *, /) равны
    //// 0 - в противоположном случае
}

// Функция для получения приоритета оператора для правильной расстановки операторов в постфиксной записи
int getPriority(char *oper) {
    if (strcmp(oper, "+") == 0 || strcmp(oper, "-") == 0) {
        return 1;
    } else if (strcmp(oper, "*") == 0 || strcmp(oper, "/") == 0) {
        return 2;
    }
    return 0; // Для операндов
}

// Функция для преобразования выражения в постфиксную запись (алгоритм сортировочной станции)
char** infixToPostfix(char *infix, int *postfixSize) {
    // Выделяем память для массива строк, который будет содержать постфиксную запись
    // Предполагаем не более 100 токенов
    char** postfix = (char**)malloc(100 * sizeof(char*));
    if (postfix == NULL) {
        perror("Ошибка выделения памяти для postfix");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 100; ++i) {
        postfix[i] = (char*)malloc(20 * sizeof(char));
        if (postfix[i] == NULL) {
            perror("Ошибка выделения памяти для postfix[i]");
            exit(EXIT_FAILURE);
        }
    }

    // Выделяет память для стека
    char** stack = (char**)malloc(100 * sizeof(char*));
    if (stack == NULL) {
        perror("Ошибка выделения памяти для stack");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 100; ++i) {
        stack[i] = (char*)malloc(20 * sizeof(char));
        if (stack[i] == NULL) {
            perror("Ошибка выделения памяти для stack[i]");
            exit(EXIT_FAILURE);
        }
    }

    int stackTop = -1; // Индекс вершины стека (пустой стек)
    int postfixIndex = 0; // Индекс для добавления токенов в постфиксную запись
    char *token = strtok(infix, " "); // Разделяем строку на токены по пробелам
    // strtok - изменяет сроку, поэтому важно передавать копию строки

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // Если токен - число (или отрицательное)
            strcpy(postfix[postfixIndex++], token);
            // Добавляем в постфиксную запись
        }  else if (isalpha(token[0])) {
            // Если токен - переменная (начинается с буквы)
            strcpy(postfix[postfixIndex++], token);
            // Добавляем в постфиксную запись
        } else if (strcmp(token, "(") == 0) {
            strcpy(stack[++stackTop], token);
            // Добавляем в стек
        } else if (strcmp(token, ")") == 0) {
            // Из стека извлекаются все операторы до открывающейся скобки и добавляются в постфиксную
            while (stackTop >= 0 && strcmp(stack[stackTop], "(") != 0) {
                strcpy(postfix[postfixIndex++], stack[stackTop--]);
            }
            if (stackTop >= 0 && strcmp(stack[stackTop], "(") == 0) {
                stackTop--; // Удаляем открывающую скобку
            } else {
                // Добавлена обработка ошибок: Непарная скобка
                fprintf(stderr, "Ошибка: Непарные скобки\n");
                // Очистка памяти и выход
                for (int i = 0; i < 100; ++i) {
                    free(stack[i]);
                }
                free(stack);
                for (int i = 0; i < 100; ++i) {
                    free(postfix[i]);
                }
                free(postfix);
                exit(EXIT_FAILURE);
            }
        } else if (isOperator(token)) {
            while (stackTop >= 0 && strcmp(stack[stackTop], "(") != 0 && getPriority(stack[stackTop]) >= getPriority(token)) {
                // Из стека извлекаются операторы с более высоким или равным приоритетом и добавляются в постфиксную запись
                strcpy(postfix[postfixIndex++], stack[stackTop--]);
            }
            strcpy(stack[++stackTop], token);
            // Затем текущий оператор добавляется в стек
        } else {
            // Добавлена обработка ошибок: неверный символ
            fprintf(stderr, "Ошибка: Неверный символ %s\n", token);
            // Очистка памяти и выход
            for (int i = 0; i < 100; ++i) {
                free(stack[i]);
            }
            free(stack);
            for (int i = 0; i < 100; ++i) {
                free(postfix[i]);
            }
            free(postfix);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " "); // Получаем следующий токен
    }

    // Оставшиеся операторы извлекаются из стека и добавляются в постфиксную запись
    while (stackTop >= 0) {
        if (strcmp(stack[stackTop], "(") == 0) {
            // Добавлена обработка ошибок: Непарная скобка
            fprintf(stderr, "Ошибка: Непарные скобки\n");
            // Очистка памяти и выход
            for (int i = 0; i < 100; ++i) {
                free(stack[i]);
            }
            free(stack);
            for (int i = 0; i < 100; ++i) {
                free(postfix[i]);
            }
            free(postfix);
            exit(EXIT_FAILURE);
        }
        strcpy(postfix[postfixIndex++], stack[stackTop--]);
    }

    *postfixSize = postfixIndex; //Размер постфиксной записи

    // Освобождаем память, выделенную под стэк
    for (int i = 0; i < 100; ++i) {
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

// Функция для преобразования a / b + c / d -> (a * d + b * c)/(b * d)
TreeNode *transformTree(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    // Рекурсивно обрабатываем левое и правое поддеревья
    TreeNode *left = transformTree(root->left);
    TreeNode *right = transformTree(root->right);


    // Проверяем текущий узел и его потомков на соответствие шаблону a / b + c / d
    // Текущий узел - +, его левый потомок - /, правый потомок - /
    if (strcmp(root->data, "+") == 0 && root->left != NULL && strcmp(root->left->data, "/") == 0 && root->right != NULL && strcmp(root->right->data, "/") == 0) {

        // Создаем копии узлов a, b, c ,d
        TreeNode *a = createNode(root->left->left->data); // createNode создает копию
        TreeNode *b = createNode(root->left->right->data);
        TreeNode *c = createNode(root->right->left->data);
        TreeNode *d = createNode(root->right->right->data);

        // Создаем новые узлы для (a * d + b * c)/(b * d)
        TreeNode *mul1 = createNode("*");
        mul1->left = a;
        mul1->right = d;

        TreeNode *mul2 = createNode("*");
        mul2->left = b;
        mul2->right = c;

        TreeNode *add = createNode("+");
        add->left = mul1;
        add->right = mul2;

        TreeNode *mul3 = createNode("*");
        mul3->left = b;
        mul3->right = d;

        TreeNode *div = createNode("/");
        div->left = add;
        div->right = mul3;


        // Освобождаем память от старых узлов и их потомков
        freeTree(root->left);
        freeTree(root->right);
        free(root);

       return div; // Возвращаем корен преобразованного поддерева
    } else {
        // Если преобразование не выполнено, возвращаем новый корень с преобразованными потомками
        TreeNode* newRoot = createNode(root->data);
        newRoot->left = left;
        newRoot->right = right;

        return newRoot;
    }
}

// Функция для печати дерева в инфиксной записи
void printInfix(TreeNode *root) {
    if (root != NULL) {
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

// Функция для печати дерева в виде дерева (с отступами)
void printTreeIndented(TreeNode *root, int level) {
    if (root == NULL) {
        return;
    }

    // Увеличиваем отступ для каждого уровня
    for (int i = 0; i < level; i++) {
        printf("    "); // 4 пробела для каждого уровня
    }

    printf("%s\n", root->data);

    // Рекурсивно печатаем правое и левое поддеревья
    printTreeIndented(root->left, level + 1);
    printTreeIndented(root->right, level + 1);
}

// Вспомогательная функция для вызова из main
void printTreeWrapper(TreeNode *root) {
    printTreeIndented(root, 0);
}

int main() {
    char expression[200];

    printf("Введите арифметическое выражение (например: a / b + c / d): ");
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
        for (int i = 0; i < 100; ++i) {
            free(postfix[i]);
        }
        free(postfix);
        return 1; // Или другой код ошибки
    }

    for (int i = 0; i < 100; ++i) {
        free(postfix[i]);
    }
    free(postfix);


    printf("Исходное выражение (дерево): \n");
    printTreeWrapper(root);
    printf("\n");

    printf("Исходное выражение (текст): ");
    printInfix(root);
    printf("\n");

    // Выполняем преобразование дерева
    TreeNode *transformedRoot = transformTree(root);

    printf("Преобразованное выражение (дерево): \n");
    printTreeWrapper(transformedRoot);
    printf("\n");

    printf("Преобразованное выражение (текст): ");
    printInfix(transformedRoot);
    printf("\n");

    // Освобождаем память, выделенную под дерево
    freeTree(transformedRoot);

    return 0;
}