#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"

static char *read_line_stdin(void)
{
    size_t cap = 256, len = 0;
    char *buf = malloc(cap);
    if (!buf)
        return NULL;
    int c;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (c == '\n')
            break;
        if (len + 1 >= cap)
        {
            cap *= 2;
            buf = realloc(buf, cap);
            if (!buf)
                return NULL;
        }
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    if (len == 0 && c == EOF)
    {
        free(buf);
        return NULL;
    }
    return buf;
}

int main()
{
    printf("Введите инфиксное выражение (допустимы + - * /, целые и идентификаторы):\n> ");
    char *line = read_line_stdin();
    if (!line)
    {
        fprintf(stderr, "Нет входа\n");
        return 1;
    }

    TokenVec infix = {}, postfix = {};
    char *err = NULL;
    if (!tokenize(line, &infix, &err))
    {
        fprintf(stderr, "Ошибка токенизации: %s\n", err);
        free(err);
        free(line);
        return 1;
    }
    if (!infix_to_postfix(&infix, &postfix, &err))
    {
        fprintf(stderr, "Ошибка разбора: %s\n", err);
        free(err);
        free_tokens(&infix);
        free(line);
        return 1;
    }

    Node *ast = build_ast_from_postfix(&postfix, &err);
    if (!ast)
    {
        fprintf(stderr, "Ошибка построения AST: %s\n", err);
        free(err);
        free_tokens(&infix);
        free_tokens(&postfix);
        free(line);
        return 1;
    }

    printf("AST → инфикс до упрощения: ");
    print_infix(ast);
    printf("\n");
    ast = simplify_tree(ast);
    printf("После упрощения: ");
    print_infix(ast);
    printf("\n");

    free_tree(ast);
    free_tokens(&infix);
    free_tokens(&postfix);
    free(line);
    return 0;
}