%{
#include "test.h"

struct zz_fun {
        const struct zz_type *type;
        struct zz_ast *(* fun)(struct zz_ast *);
};

int serialize_fun(struct zz_ast *a, FILE *f)
{
        struct zz_fun *x = (void *)a;
        return fprintf(f, "<fun:%p>", x->fun);
}
const struct zz_type *zz_fun_type()
{
        static struct zz_type type = {
                .serialize = serialize_fun
        };
        return &type;
}
struct zz_ast *zz_fun(struct zz_ast*(*fun)(struct zz_ast*))
{
        struct zz_fun *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_fun_type();
        a->fun = fun;
        return (void *)a;
}

struct zz_ast *eval(struct zz_ast *a)
{
        if (a->type != zz_pair_type())
                return a;
        if (zz_head(a)->type != zz_fun_type())
                return a;
        return zz_cast(zz_fun, zz_head(a))->fun(zz_tail(a));
}

struct zz_ast *op_set(struct zz_ast *a)
{
        struct zz_ast *l = eval(zz_head(a));
        struct zz_ast *r = eval(zz_head(zz_tail(a)));
        return r;
}
struct zz_ast *op_gt(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num > r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_lt(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num < r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_ge(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num >= r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_le(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num <= r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_eq(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num == r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_ne(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r && l->num != r->num ? zz_int(1) : NULL;
}
struct zz_ast *op_add(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num + r->num) : NULL;
}
struct zz_ast *op_sub(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num - r->num) : NULL;
}
struct zz_ast *op_mul(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num * r->num) : NULL;
}
struct zz_ast *op_div(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num / r->num) : NULL;
}
struct zz_ast *op_mod(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num % r->num) : NULL;
}
struct zz_ast *op_exp(struct zz_ast *a)
{
        struct zz_int *l = zz_cast(zz_int, eval(zz_head(a)));
        struct zz_int *r = zz_cast(zz_int, eval(zz_head(zz_tail(a))));
        return l && r ? zz_int(l->num * r->num) : NULL;
}

%}

%define api.pure full
%param {FILE *f}
%define api.value.type {struct zz_ast *}

/*
 * Token and rule declarations
 */
%token '\n'
%token '='
%token '+'
%token '-'
%token '*'
%token '/'
%token '%'
%token '('
%token ')'
%token '<'
%token '>'
%token OP_EQ
%token OP_NE
%token OP_GE
%token OP_LE
%token ATOM

%%

input
    : input '\n' line
    | line
    ;

line
    : statement_list {
        struct zz_ast *r = prune($1, 1);
        zz_print(r, stdout);
        fprintf(stdout, " = ");
        zz_print(eval(r), stdout);
        fprintf(stdout, "\n");
        }
    |
    ;

statement_list
    : statement ';' statement_list {
        $$ = zz_pair($1, $3);
        }
    | statement {
        $$ = zz_pair($1, NULL);
        }
    | {
        $$ = NULL;
        }
    ;

statement
    : expression statement {
        $$ = zz_pair($1, $2);
        }
    | {
        $$ = NULL;
        }
    ;

expression
   : assignment_expression { $$ = $1; }
   ;

assignment_expression
    : comparative_expression assignment_operator assignment_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | comparative_expression {
        $$ = $1;
        }
    ;

assignment_operator
    : '=' { $$ = zz_fun(op_set); }
    ;

comparative_expression
    : additive_expression comparative_operator comparative_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | additive_expression {
        $$ = $1;
        }
    ;

comparative_operator
    : '>' { $$ = zz_fun(op_gt); }
    | '<' { $$ = zz_fun(op_lt); }
    | OP_EQ { $$ = zz_fun(op_eq); }
    | OP_NE { $$ = zz_fun(op_ne); }
    | OP_GE { $$ = zz_fun(op_ge); }
    | OP_LE { $$ = zz_fun(op_le); }
    ;

additive_expression
    : multiplicative_expression additive_operator additive_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | multiplicative_expression {
        $$ = $1;
        }
    ;

additive_operator
    : '+' { $$ = zz_fun(op_add); }
    | '-' { $$ = zz_fun(op_sub); }
    ;

multiplicative_expression
    : exponential_expression multiplicative_operator multiplicative_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | exponential_expression {
        $$ = $1;
        }
    ;

multiplicative_operator
    : '*' { $$ = zz_fun(op_mul); }
    | '/' { $$ = zz_fun(op_div); }
    | '%' { $$ = zz_fun(op_mod); }
    ;

exponential_expression
    : atomic_expression exponential_operator exponential_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | atomic_expression {
        $$ = $1;
        }
    ;

exponential_operator
    : '^' { $$ = zz_fun(op_exp); }
    ;

atomic_expression
    : ATOM { $$ = $1; }
    | '(' statement_list ')' { $$ = $2; }
    ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, FILE *f)
{
        static int len = 0;
        static int cap = 0;
        static char *buf = NULL;

        for (;;) {
                int c = fgetc(f);
                switch (c) {
                 case EOF:
                        return 0;
                 case ' ':
                 case '\t':
                 case '\n':
                 case '\r':
                        break;
                 case 'a'...'z':
                 case 'A'...'Z':
                 case '_':
                        len = 0;
                        do {
                                if (len == cap) {
                                        cap = cap ? cap * 2 : 2;
                                        buf = realloc(buf, cap);
                                }
                                buf[len++] = c;
                                c = fgetc(f);
                        } while (c == '_' || isalnum(c));
                        ungetc(c, f);
                        *lvalp = zz_str_with_len(buf, len);
                        return ATOM;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        *lvalp = zz_int(c);
                        return ATOM;
                 case '"':
                        c = fgetc(f);
                        len = 0;
                        while (c != '"') {
                                if (c == EOF) {
                                        fprintf(stderr, "Unterminated string\n");
                                        abort();
                                }
                                if (len == cap) {
                                        cap = cap ? cap * 2 : 2;
                                        buf = realloc(buf, cap);
                                }
                                buf[len++] = c;
                                c = fgetc(f);
                        }
                        *lvalp = zz_str_with_len(buf, len);
                        return ATOM;
                 case '=':
                        c = fgetc(f);
                        if (c == '=')
                                return OP_EQ;
                        ungetc(c, f);
                        return '=';
                 case '>':
                        c = fgetc(f);
                        if (c == '=')
                                return OP_GE;
                        ungetc(c, f);
                        return '>';
                 case '<':
                        c = fgetc(f);
                        if (c == '=')
                                return OP_LE;
                        ungetc(c, f);
                        return '<';
                 case '!':
                        c = fgetc(f);
                        if (c == '=')
                                return OP_NE;
                        ungetc(c, f);
                        return '!';
                 default:
                        return c;
                }
        }
}

