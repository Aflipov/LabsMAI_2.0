#include <stdio.h>
#include "expr.h"

int main() {
    // Построим вручную дерево для (a*b*5)/(a*10)
    // Node* expr =
    //     create_node("/",
    //         create_node("*",
    //             create_node("*",
    //                 create_node("a", NULL, NULL),
    //                 create_node("b", NULL, NULL)),
    //             create_node("5", NULL, NULL)),
    //         create_node("*",
    //             create_node("a", NULL, NULL),
    //             create_node("10", NULL, NULL))
    //     );

    Node* expr = 
        create_node("/",
            create_node("a", NULL, NULL),
            create_node("a", NULL, NULL)
        );

    printf("Исходное выражение: ");
    print_infix(expr);
    printf("\n");

    expr = simplify_tree(expr);

    printf("Упрощённое выражение: ");
    print_infix(expr);
    printf("\n");

    free_tree(expr);
    return 0;
}
