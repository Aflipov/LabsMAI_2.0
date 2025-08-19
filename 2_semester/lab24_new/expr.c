#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---------- small utils ----------
static void *xmalloc(size_t n)
{
    void *p = malloc(n);
    if (!p)
    {
        fprintf(stderr, "OOM\n");
        exit(1);
    }
    return p;
}
static char *xstrdup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = xmalloc(n);
    memcpy(p, s, n);
    return p;
}

static void tv_init(TokenVec *v)
{
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}
static void tv_push(TokenVec *v, Token t)
{
    if (v->size == v->cap)
    {
        v->cap = v->cap ? v->cap * 2 : 16;
        v->data = realloc(v->data, v->cap * sizeof(Token));
        if (!v->data)
        {
            fprintf(stderr, "OOM\n");
            exit(1);
        }
    }
    v->data[v->size++] = t;
}
void free_tokens(TokenVec *v)
{
    for (int i = 0; i < v->size; i++)
        free(v->data[i].lex);
    free(v->data);
    v->data = NULL;
    v->size = v->cap = 0;
}

// ---------- Tokenizer ----------
static int is_ident_start(int c) { return isalpha(c) || c == '_'; }
static int is_ident(int c) { return isalnum(c) || c == '_'; }

int tokenize(const char *s, TokenVec *out, char **err)
{
    tv_init(out);
    for (size_t i = 0; s[i];)
    {
        if (isspace((unsigned char)s[i]))
        {
            i++;
            continue;
        }
        if (isdigit((unsigned char)s[i]))
        {
            size_t j = i;
            while (isdigit((unsigned char)s[j]))
                j++;
            char *lex = xmalloc(j - i + 1);
            memcpy(lex, s + i, j - i);
            lex[j - i] = '\0';
            tv_push(out, (Token){TOK_NUM, lex});
            i = j;
            continue;
        }
        if (is_ident_start((unsigned char)s[i]))
        {
            size_t j = i;
            while (is_ident((unsigned char)s[j]))
                j++;
            char *lex = xmalloc(j - i + 1);
            memcpy(lex, s + i, j - i);
            lex[j - i] = '\0';
            tv_push(out, (Token){TOK_ID, lex});
            i = j;
            continue;
        }
        char c = s[i];
        if (c == '(')
        {
            tv_push(out, (Token){TOK_LPAREN, xstrdup("(")});
            i++;
            continue;
        }
        if (c == ')')
        {
            tv_push(out, (Token){TOK_RPAREN, xstrdup(")")});
            i++;
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            char buf[2] = {c, '\0'};
            tv_push(out, (Token){TOK_OP, xstrdup(buf)});
            i++;
            continue;
        }
        *err = xstrdup("Недопустимый символ в выражении");
        free_tokens(out);
        return 0;
    }
    return 1;
}

// ---------- AST constructors ----------
Node *node_num_str(const char *s)
{
    Node *n = xmalloc(sizeof(Node));
    n->kind = N_NUM;
    n->val = xstrdup(s);
    n->l = n->r = NULL;
    return n;
}
Node *node_id(const char *s)
{
    Node *n = xmalloc(sizeof(Node));
    n->kind = N_ID;
    n->val = xstrdup(s);
    n->l = n->r = NULL;
    return n;
}
Node *node_op(const char *op, Node *l, Node *r)
{
    Node *n = xmalloc(sizeof(Node));
    n->kind = N_OP;
    n->val = xstrdup(op);
    n->l = l;
    n->r = r;
    return n;
}
void free_tree(Node *n)
{
    if (!n)
        return;
    free_tree(n->l);
    free_tree(n->r);
    free(n->val);
    free(n);
}

// ---------- precedence/assoc ----------
static int prec(const char *op)
{
    switch (op[0])
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return -1;
    }
}
static int is_left_assoc(const char *op)
{
    (void)op;
    return 1;
}

// ---------- Shunting-yard ----------
int infix_to_postfix(const TokenVec *infix, TokenVec *postfix, char **err)
{
    tv_init(postfix);
    TokenVec stack;
    tv_init(&stack);
    for (int i = 0; i < infix->size; i++)
    {
        Token t = infix->data[i];
        if (t.type == TOK_NUM || t.type == TOK_ID)
        {
            tv_push(postfix, (Token){t.type, xstrdup(t.lex)});
        }
        else if (t.type == TOK_OP)
        {
            while (stack.size > 0)
            {
                Token top = stack.data[stack.size - 1];
                if (top.type == TOK_OP && ((is_left_assoc(t.lex) && prec(t.lex) <= prec(top.lex)) || (!is_left_assoc(t.lex) && prec(t.lex) < prec(top.lex))))
                {
                    tv_push(postfix, (Token){top.type, xstrdup(top.lex)});
                    stack.size--;
                }
                else
                    break;
            }
            tv_push(&stack, (Token){TOK_OP, xstrdup(t.lex)});
        }
        else if (t.type == TOK_LPAREN)
        {
            tv_push(&stack, (Token){TOK_LPAREN, xstrdup("(")});
        }
        else if (t.type == TOK_RPAREN)
        {
            int matched = 0;
            while (stack.size > 0)
            {
                Token top = stack.data[--stack.size];
                if (top.type == TOK_LPAREN)
                {
                    free(top.lex);
                    matched = 1;
                    break;
                }
                tv_push(postfix, (Token){top.type, xstrdup(top.lex)});
                free(top.lex);
            }
            if (!matched)
            {
                *err = xstrdup("Несогласованные скобки");
                free_tokens(postfix);
                free_tokens(&stack);
                return 0;
            }
        }
    }
    while (stack.size > 0)
    {
        Token top = stack.data[--stack.size];
        if (top.type == TOK_LPAREN || top.type == TOK_RPAREN)
        {
            *err = xstrdup("Несогласованные скобки");
            free(top.lex);
            free_tokens(postfix);
            free_tokens(&stack);
            return 0;
        }
        tv_push(postfix, (Token){top.type, xstrdup(top.lex)});
        free(top.lex);
    }
    free(stack.data);
    return 1;
}

// ---------- Build AST from postfix ----------
typedef struct
{
    Node **data;
    int size;
    int cap;
} NodeStack;
static void ns_init(NodeStack *s)
{
    s->data = NULL;
    s->size = 0;
    s->cap = 0;
}
static void ns_push(NodeStack *s, Node *n)
{
    if (s->size == s->cap)
    {
        s->cap = s->cap ? s->cap * 2 : 16;
        s->data = realloc(s->data, s->cap * sizeof(Node *));
        if (!s->data)
        {
            fprintf(stderr, "OOM\n");
            exit(1);
        }
    }
    s->data[s->size++] = n;
}
static Node *ns_pop(NodeStack *s)
{
    if (s->size == 0)
        return NULL;
    return s->data[--s->size];
}

Node *build_ast_from_postfix(const TokenVec *postfix, char **err)
{
    NodeStack st;
    ns_init(&st);
    for (int i = 0; i < postfix->size; i++)
    {
        Token t = postfix->data[i];
        if (t.type == TOK_NUM)
        {
            ns_push(&st, node_num_str(t.lex));
        }
        else if (t.type == TOK_ID)
        {
            ns_push(&st, node_id(t.lex));
        }
        else if (t.type == TOK_OP)
        {
            Node *r = ns_pop(&st), *l = ns_pop(&st);
            if (!l || !r)
            {
                *err = xstrdup("Синтаксическая ошибка: не хватает операндов");
                free(st.data);
                return NULL;
            }
            ns_push(&st, node_op(t.lex, l, r));
        }
    }
    if (st.size != 1)
    {
        *err = xstrdup("Синтаксическая ошибка: лишние операнды/операторы");
        free(st.data);
        return NULL;
    }
    Node *root = ns_pop(&st);
    free(st.data);
    return root;
}

// ---------- Simplification helpers ----------
static int is_num_node(Node *n) { return n && n->kind == N_NUM; }
static int is_id_node(Node *n) { return n && n->kind == N_ID; }
static int is_op(Node *n, char c) { return n && n->kind == N_OP && n->val[0] == c && n->val[1] == '\0'; }
static int str_to_int(const char *s) { return atoi(s); }
int is_number(const char* s) {
    if (!s || !*s) return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}
static char *int_to_str(int v)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d", v);
    return xstrdup(buf);
}
static int gcd_int(int a, int b)
{
    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    while (b)
    {
        int t = a % b;
        a = b;
        b = t;
    }
    return a ? a : 1;
}

// Collect factors for '*' subtree into dynamic array of Node* (borrowing existing nodes)
typedef struct
{
    Node **data;
    int size;
    int cap;
} NVec;
static void nv_init(NVec *v)
{
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}
static void nv_push(NVec *v, Node *n)
{
    if (v->size == v->cap)
    {
        v->cap = v->cap ? v->cap * 2 : 8;
        v->data = realloc(v->data, v->cap * sizeof(Node *));
        if (!v->data)
        {
            fprintf(stderr, "OOM\n");
            exit(1);
        }
    }
    v->data[v->size++] = n;
}

static void collect_mul(Node *n, NVec *out)
{
    if (is_op(n, '*'))
    {
        collect_mul(n->l, out);
        collect_mul(n->r, out);
    }
    else
        nv_push(out, n);
}

static Node *rebuild_mul(NVec *v)
{
    if (v->size == 0)
        return NULL;
    Node *res = v->data[0];
    for (int i = 1; i < v->size; i++)
        res = node_op("*", res, v->data[i]);
    return res;
}

// Simplify simple algebraic identities for + and * (with integers only)
static Node *simplify_basic(Node *n)
{
    if (is_op(n, '*'))
    {
        // 0*x or x*0 => 0; 1*x => x; x*1 => x; both numbers => multiply
        if (is_num_node(n->l) && strcmp(n->l->val, "0") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->r) && strcmp(n->r->val, "0") == 0)
        {
            return n->r;
        }
        if (is_num_node(n->l) && strcmp(n->l->val, "1") == 0)
        {
            return n->r;
        }
        if (is_num_node(n->r) && strcmp(n->r->val, "1") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->l) && is_num_node(n->r))
        {
            int a = str_to_int(n->l->val), b = str_to_int(n->r->val);
            Node *res = node_num_str(int_to_str(a * b));
            return res;
        }
    }
    if (is_op(n, '+'))
    {
        // x+0 => x; 0+x => x; both numbers => add
        if (is_num_node(n->l) && strcmp(n->l->val, "0") == 0)
        {
            return n->r;
        }
        if (is_num_node(n->r) && strcmp(n->r->val, "0") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->l) && is_num_node(n->r))
        {
            int a = str_to_int(n->l->val), b = str_to_int(n->r->val);
            Node *res = node_num_str(int_to_str(a + b));
            return res;
        }
    }
    if (is_op(n, '-'))
    {
        if (is_num_node(n->r) && strcmp(n->r->val, "0") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->l) && is_num_node(n->r))
        {
            int a = str_to_int(n->l->val), b = str_to_int(n->r->val);
            Node *res = node_num_str(int_to_str(a - b));
            return res;
        }
    }
    if (is_op(n, '/'))
    {
        if (is_num_node(n->r) && strcmp(n->r->val, "1") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->l) && strcmp(n->l->val, "0") == 0)
        {
            return n->l;
        }
        if (is_num_node(n->l) && is_num_node(n->r))
        {
            int a = str_to_int(n->l->val), b = str_to_int(n->r->val);
            if (b != 0)
            {
                int g = gcd_int(a, b);
                a /= g;
                b /= g;
                if (b == 1)
                    return node_num_str(int_to_str(a));
                return node_op("/", node_num_str(int_to_str(a)), node_num_str(int_to_str(b)));
            }
        }
    }
    return n; // no basic rule applies
}

// Fraction reduction: (mul...)/(mul...)
static Node *simplify_fraction(Node *n)
{
    if (!is_op(n, '/'))
        return n;
    // collect factors
    NVec num, den;
    nv_init(&num);
    nv_init(&den);
    collect_mul(n->l, &num);
    collect_mul(n->r, &den);

    // cancel identical symbols (IDs)
    for (int i = 0; i < num.size; i++)
    {
        Node *a = num.data[i];
        if (!a)
            continue;
        if (is_id_node(a))
        {
            for (int j = 0; j < den.size; j++)
            {
                Node *b = den.data[j];
                if (!b)
                    continue;
                if (is_id_node(b) && strcmp(a->val, b->val) == 0)
                {
                    // replace both by 1
                    free(a->val);
                    a->val = xstrdup("1");
                    free(b->val);
                    b->val = xstrdup("1");
                    break;
                }
            }
        }
    }

    // reduce integer factors using gcd in a nested manner
    for (int i = 0; i < num.size; i++)
    {
        Node *a = num.data[i];
        if (!a || !is_num_node(a))
            continue;
        int ai = str_to_int(a->val);
        for (int j = 0; j < den.size; j++)
        {
            Node *b = den.data[j];
            if (!b || !is_num_node(b))
                continue;
            int bi = str_to_int(b->val);
            int g = gcd_int(ai, bi);
            if (g > 1)
            {
                ai /= g;
                bi /= g;
                free(a->val);
                a->val = int_to_str(ai);
                free(b->val);
                b->val = int_to_str(bi);
            }
        }
    }

    // remove factors equal to 1
    NVec num2;
    nv_init(&num2);
    for (int i = 0; i < num.size; i++)
    {
        if (num.data[i] && !(is_number(num.data[i]->val) && strcmp(num.data[i]->val, "1") == 0))
            nv_push(&num2, num.data[i]);
        else if(num.data[i]) // Комментарий: Освобождение памяти для узлов со значением "1"
        {
         free_tree(num.data[i]); // Комментарий: Освобождаем память
        }
    }
    NVec den2;
    nv_init(&den2);
    for (int j = 0; j < den.size; j++)
    {
        if (den.data[j] && !(is_number(den.data[j]->val) && strcmp(den.data[j]->val, "1") == 0))
            nv_push(&den2, den.data[j]);
        else if(den.data[j]) // Комментарий: Освобождение памяти для узлов со значением "1"
        {
         free_tree(den.data[j]); // Комментарий: Освобождаем память
        }
    }

    Node *new_num = num2.size ? rebuild_mul(&num2) : node_num_str("1");
    Node *new_den = den2.size ? rebuild_mul(&den2) : NULL; // if empty denominator → whole expr is numerator

    free(num.data);
    free(den.data);
    free(num2.data);
    free(den2.data);

    if (!new_den)
    {
      //free_tree(n->r); // Комментарий: Удаление старого правого поддерева (знаменателя)
      //free(n->val);   // Комментарий: Освобождаем память, выделенную под оператор
      //free(n);       // Комментарий: Освобождаем память, выделенную под узел
      return new_num;
    }
    //free_tree(n->l); // Комментарий: Удаление старого левого поддерева (числителя)
    //free(n->val);  // Комментарий: Освобождаем память, выделенную под оператор
    //n->kind = N_OP;   // Комментарий: Устанавливаем тип узла обратно в оператор
    //n->val = xstrdup("/"); // Комментарий: Устанавливаем оператор деления
    //n->l = new_num;  // Комментарий: Устанавливаем новый числитель
    //n->r = new_den;  // Комментарий: Устанавливаем новый знаменатель
    return node_op("/", new_num, new_den);
    //return n;
}

// Main simplify (post-order): first simplify children, then apply fraction & basic rules
Node *simplify_tree(Node *root)
{
    if (!root)
        return NULL;
    root->l = simplify_tree(root->l);
    root->r = simplify_tree(root->r);
    if (is_op(root, '/'))
    {
        root = simplify_fraction(root);
    }
    // after fraction, try basic algebraic identities
    Node *applied = simplify_basic(root);
    return applied;
}

// ---------- Printing with minimal parentheses ----------
static int node_prec(Node *n)
{
    if (!n || n->kind != N_OP)
        return 3;
    switch (n->val[0])
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 3;
    }
}
static void print_rec(Node *n, int parent_prec, int is_right)
{
    if (!n)
        return;
    if (n->kind != N_OP)
    {
        printf("%s", n->val);
        return;
    }
    int myp = node_prec(n);
    int need_paren = myp < parent_prec || (is_right && (n->val[0] == '-' || n->val[0] == '/'));
    if (need_paren)
        printf("(");
    print_rec(n->l, myp, 0);
    printf(" %s ", n->val);
    print_rec(n->r, myp, 1);
    if (need_paren)
        printf(")");
}
void print_infix(Node *n) { print_rec(n, 0, 0); }
