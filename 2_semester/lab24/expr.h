#ifndef EXPR_H
#define EXPR_H
#include <stddef.h>

// ---- Tokens ----
typedef enum
{
    TOK_NUM,
    TOK_ID,
    TOK_OP,
    TOK_LPAREN,
    TOK_RPAREN
} TokType;

typedef struct
{
    TokType type;
    char *lex; // string for number or identifier or operator
} Token;

typedef struct
{
    Token *data;
    int size;
    int cap;
} TokenVec;

// Tokenization
int tokenize(const char *s, TokenVec *out, char **err);
void free_tokens(TokenVec *v);

// ---- AST ----
typedef enum
{
    N_NUM,
    N_ID,
    N_OP
} NodeKind;

typedef struct Node
{
    NodeKind kind;
    char *val;          // for N_NUM (int string), N_ID (name), N_OP (operator symbol "+-*/")
    struct Node *l, *r; // children for operator nodes
} Node;

Node *node_num_str(const char *s);
Node *node_id(const char *s);
Node *node_op(const char *op, Node *l, Node *r);
void free_tree(Node *n);

// ---- Infix → Postfix (Shunting-yard) ----
int infix_to_postfix(const TokenVec *infix, TokenVec *postfix, char **err);

// ---- Build AST from postfix ----
Node *build_ast_from_postfix(const TokenVec *postfix, char **err);

// ---- Simplification ----
Node *simplify_tree(Node *root); // returns possibly new root (old nodes reused)

// ---- Printing ----
void print_infix(Node *n);

#endif // EXPR_H
