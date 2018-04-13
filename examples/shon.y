%{
#include "test.h"

struct var {
        struct zz_ast *key;
        struct zz_ast *val;
};

static struct array env = array(sizeof(struct var));

struct zz_fun {
        const struct zz_type *type;
        struct zz_ast *(* fun)(struct array *, struct zz_ast *);
};

const struct zz_type *zz_fun_type(void);
struct zz_ast *zz_fun(struct zz_ast*(*fun)(struct array *, struct zz_ast*));

struct zz_sym {
        const struct zz_type *type;
        char sym[];
};

const struct zz_type *zz_sym_type(void);
struct zz_ast *zz_sym_with_len(const char *str, int len);
struct zz_ast *zz_sym(const char *str);

struct zz_ast *eval(struct array *env, struct zz_ast *a);

struct zz_ast *op_set(struct array *env, struct zz_ast *a);
struct zz_ast *op_gt(struct array *env, struct zz_ast *a);
struct zz_ast *op_lt(struct array *env, struct zz_ast *a);
struct zz_ast *op_le(struct array *env, struct zz_ast *a);
struct zz_ast *op_ge(struct array *env, struct zz_ast *a);
struct zz_ast *op_eq(struct array *env, struct zz_ast *a);
struct zz_ast *op_ne(struct array *env, struct zz_ast *a);
struct zz_ast *op_add(struct array *env, struct zz_ast *a);
struct zz_ast *op_sub(struct array *env, struct zz_ast *a);
struct zz_ast *op_mul(struct array *env, struct zz_ast *a);
struct zz_ast *op_div(struct array *env, struct zz_ast *a);
struct zz_ast *op_mod(struct array *env, struct zz_ast *a);
struct zz_ast *op_exp(struct array *env, struct zz_ast *a);
struct zz_ast *op_dots(struct array *env, struct zz_ast *a);

struct zz_ast *op_echo(struct array *env, struct zz_ast *a);
struct zz_ast *op_env(struct array *env, struct zz_ast *a);
struct zz_ast *op_if(struct array *env, struct zz_ast *a);
struct zz_ast *op_while(struct array *env, struct zz_ast *a);
struct zz_ast *op_for(struct array *env, struct zz_ast *a);
struct zz_ast *op_case(struct array *env, struct zz_ast *a);

%}

%define api.pure full
%param {FILE *f}
%error-verbose

%union {
        struct zz_ast *ast;
        struct zz_list list;
}

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
%token OP_DOTS
%token<ast> NUMBER
%token<ast> STRING
%token<ast> SYMBOL

%type<ast> program
%type<list> statement_list
%type<ast> statement
%type<list> expression_list
%type<ast> expression
%type<ast> assignment_expression
%type<ast> assignment_operator
%type<ast> relational_expression
%type<ast> relational_operator
%type<ast> additive_expression
%type<ast> additive_operator
%type<ast> multiplicative_expression
%type<ast> multiplicative_operator
%type<ast> exponential_expression
%type<ast> exponential_operator
%type<ast> range_expression
%type<ast> range_operator
%type<ast> primary_expression

%initial-action {
        if (env.len == 0) {
                {
                        struct var x = {zz_sym("echo"), zz_fun(op_echo)};
                        array_push_back(&env, &x);
                }
                {
                        struct var x = {zz_sym("env"), zz_fun(op_env)};
                        array_push_back(&env, &x);
                }
                {
                        struct var x = {zz_sym("if"), zz_fun(op_if)};
                        array_push_back(&env, &x);
                }
                {
                        struct var x = {zz_sym("while"), zz_fun(op_while)};
                        array_push_back(&env, &x);
                }
                {
                        struct var x = {zz_sym("for"), zz_fun(op_for)};
                        array_push_back(&env, &x);
                }
                {
                        struct var x = {zz_sym("case"), zz_fun(op_case)};
                        array_push_back(&env, &x);
                }
        }
}
%%

input
        : input '\n' line
        | line
        ;

line
        : program {
                struct zz_ast *s = eval(&env, $1);
                        zz_print($1, stdout);
                        fprintf(stdout, "\n");
                if (s) {
                        zz_print(s, stdout);
                        fprintf(stdout, "\n");
                }
        }
        |
        ;

program
        : statement_list {
                /* if the list contains only one element, pass it directly */
                $$ = $1.first == $1.last ? zz_head($1.first) : $1.first;
        }
        ;

statement_list
        : statement_list ';' statement { $$ = zz_append($1, $3); }
        | statement { $$ = zz_list($1); }
        | { $$ = zz_list_empty(); }
        ;

statement
        : expression_list { 
                /* if the list contains only one element, pass it directly */
                $$ = $1.first == $1.last ? zz_head($1.first) : $1.first;
        }
        ;

expression_list
        : expression_list expression { $$ = zz_append($1, $2); }
        | { $$ = zz_list_empty(); }
        ;

expression
        : assignment_expression
        ;

assignment_expression
        : assignment_expression assignment_operator relational_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | assignment_expression assignment_operator {
                $$ = zz_list($2, $1).first;
        }
        | relational_expression
        ;

assignment_operator
        : '=' { $$ = zz_fun(op_set); }
        ;

relational_expression
        : relational_expression relational_operator additive_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | additive_expression
        ;

relational_operator
        : '>' { $$ = zz_fun(op_gt); }
        | '<' { $$ = zz_fun(op_lt); }
        | OP_EQ { $$ = zz_fun(op_eq); }
        | OP_NE { $$ = zz_fun(op_ne); }
        | OP_GE { $$ = zz_fun(op_ge); }
        | OP_LE { $$ = zz_fun(op_le); }
        ;

additive_expression
        : additive_expression additive_operator multiplicative_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | multiplicative_expression
        ;

additive_operator
        : '+' { $$ = zz_fun(op_add); }
        | '-' { $$ = zz_fun(op_sub); }
        ;

multiplicative_expression
        : multiplicative_expression multiplicative_operator exponential_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | exponential_expression
        ;

multiplicative_operator
        : '*' { $$ = zz_fun(op_mul); }
        | '/' { $$ = zz_fun(op_div); }
        | '%' { $$ = zz_fun(op_mod); }
        ;

exponential_expression
        : exponential_expression exponential_operator range_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | range_expression
        ;

exponential_operator
        : '^' { $$ = zz_fun(op_exp); }
        ;

range_expression
        : range_expression range_operator primary_expression {
                $$ = zz_list($2, $1, $3).first;
        }
        | primary_expression
        ;

range_operator
        : OP_DOTS { $$ = zz_fun(op_dots); }
        ;

primary_expression
        : NUMBER
        | STRING
        | SYMBOL
        | '(' program ')' { $$ = $2; }
        ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, FILE *f)
{
        static struct array buf = array(1);

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
                        array_clear(&buf);
                        do {
                                array_push_back(&buf, &c);
                                c = fgetc(f);
                        } while (c == '_' || isalnum(c));
                        ungetc(c, f);
                        lvalp->ast = zz_sym_with_len(buf.buf, buf.len);
                        return SYMBOL;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        lvalp->ast = zz_int(c);
                        return NUMBER;
                 case '"':
                        c = fgetc(f);
                        array_clear(&buf);
                        while (c != '"') {
                                if (c == EOF) {
                                        fprintf(stderr, "Unterminated string\n");
                                        abort();
                                }
                                array_push_back(&buf, &c);
                                c = fgetc(f);
                        }
                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                        return STRING;
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
                 case '.':
                        c = fgetc(f);
                        if (c == '.')
                                return OP_DOTS;
                        ungetc(c, f);
                        return '.';
                 default:
                        return c;
                }
        }
}

int serialize_fun(struct zz_ast *a, FILE *f)
{
        struct zz_fun *x = (void *)a;
        return fprintf(f, "<fun:%p>", x->fun);
}
struct zz_ast *copy_fun(struct zz_ast *a)
{
        return a;
}
int cmp_fun(struct zz_ast *a, struct zz_ast *b)
{
        return b - a;
}
const struct zz_type *zz_fun_type(void)
{
        static struct zz_type type = {
                .name = "zz_fun",
                .serialize = serialize_fun,
                .copy = copy_fun,
                .cmp = cmp_fun,
        };
        return &type;
}
struct zz_ast *zz_fun(struct zz_ast*(*fun)(struct array *, struct zz_ast*))
{
        struct zz_fun *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_fun_type();
        a->fun = fun;
        return (void *)a;
}

int serialize_sym(struct zz_ast *a, FILE *f)
{
        struct zz_sym *x = (void *)a;
        return fprintf(f, "%s", x->sym);
}
struct zz_ast *copy_sym(struct zz_ast *a)
{
        return a;
}
int cmp_sym(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_sym *x = (void *)a;
        struct zz_sym *y = (void *)b;
        return strcmp(x->sym, y->sym);
}
const struct zz_type *zz_sym_type(void)
{
        static struct zz_type type = {
                .name = "zz_sym",
                .serialize = serialize_sym,
                .copy = copy_sym,
                .cmp = cmp_sym,
        };
        return &type;
}
struct zz_ast *zz_sym_with_len(const char *str, int len)
{
        struct zz_sym *a = GC_malloc_atomic(sizeof(*a) + len + 1);
        a->type = zz_sym_type();
        memcpy(a->sym, str, len);
        a->sym[len] = 0;
        return (void *)a;
}
struct zz_ast *zz_sym(const char *str)
{
        return zz_sym_with_len(str, strlen(str));
}

struct zz_ast *eval(struct array *env, struct zz_ast *a)
{
        if (zz_typeof(a) == zz_sym_type()) {
                for (int i = 0; i < env->len; ++i) {
                        struct var *x = array_index(env, i);
                        if (zz_cmp(x->key, a) == 0)
                                return x->val;
                }
                return NULL;
        } else if (zz_typeof(a) == zz_pair_type()) {
                struct zz_ast *h = eval(env, zz_head(a));
                if (zz_typeof(h) == zz_fun_type())
                        return zz_cast(zz_fun, h)->fun(env, zz_tail(a));
                struct zz_list l = zz_list(h);
                struct zz_ast *it;
                zz_foreach(it, zz_tail(a))
                        l = zz_append(l, eval(env, it));
                return l.first;
        }
        return a;
}

struct zz_ast *op_set(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r = NULL;
        if (zz_unpack(a, &l, &r) < 1)
                return NULL;
        if (zz_typeof(l) != zz_sym_type()) {
                fprintf(stderr, "expected l-value: ");
                zz_print(l, stderr);
                fprintf(stderr, "\n");
                return NULL;
        }
        r = eval(env, r);
        for (int i = 0; i < env->len; ++i) {
                struct var *x = array_index(env, i);
                if (zz_cmp(x->key, l) == 0) {
                        if (r == NULL)
                                array_erase(env, i);
                        else
                                x->val = r;
                        return r;
                }
        }
        if (r != NULL) {
                struct var x = {l, r};
                array_push_back(env, &x);
        }
        return r;
}
struct zz_ast *op_gt(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) > 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_lt(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) < 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_ge(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) >= 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_le(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) <= 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_eq(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) == 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_ne(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        return zz_cmp(eval(env, l), eval(env, r)) != 0 ? zz_int(1) : NULL;
}
struct zz_ast *op_add(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num + y->num);
 fail:
        fprintf(stderr, "usage: number + number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_sub(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num - y->num);
 fail:
        fprintf(stderr, "usage: number - number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_mul(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num * y->num);
 fail:
        fprintf(stderr, "usage: number * number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_div(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num / y->num);
 fail:
        fprintf(stderr, "usage: number / number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_mod(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num % y->num);
 fail:
        fprintf(stderr, "usage: number %% number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_exp(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        return zz_int(x->num * y->num);
 fail:
        fprintf(stderr, "usage: number ^ number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_dots(struct array *env, struct zz_ast *a)
{
        struct zz_ast *l, *r;
        zz_unpack(a, &l, &r);
        struct zz_int *x = zz_cast_or_null(zz_int, eval(env, l));
        if (x == NULL)
                goto fail;
        struct zz_int *y = zz_cast_or_null(zz_int, eval(env, r));
        if (y == NULL)
                goto fail;
        struct zz_list z = zz_list_empty();
        if (x->num < y->num) {
                for (int i = x->num; i <= y->num; ++i)
                        z = zz_append(z, zz_int(i));
        } else {
                for (int i = x->num; i >= y->num; --i)
                        z = zz_append(z, zz_int(i));
        }
        return z.first;
 fail:
        fprintf(stderr, "usage: number .. number, got ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_echo(struct array *env, struct zz_ast *a)
{
        struct zz_ast *it;
        int first = 1;
        zz_foreach(it, a) {
                if (first)
                        first = 0;
                else
                        printf(" ");
                zz_print(eval(env, it), stdout);
        }
        printf("\n");
        return NULL;
}
struct zz_ast *op_env(struct array *env, struct zz_ast *a)
{
        for (int i = 0; i < env->len; ++i) {
                struct var *x = array_index(env, i);
                zz_print(x->key, stdout);
                printf("=");
                zz_print(x->val, stdout);
                printf("\n");
        }
        return NULL;
}
struct zz_ast *op_if(struct array *env, struct zz_ast *a)
{
        struct zz_ast *cond, *stmt, *other = NULL;
        if (zz_unpack(a, &cond, &stmt, &other) < 2)
                goto fail;
        if (eval(env, cond))
                return eval(env, stmt);
        return eval(env, other);
 fail:
        fprintf(stderr, "usage: if cond stmt [other]: ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_while(struct array *env, struct zz_ast *a)
{
        struct zz_ast *cond, *stmt;
        if (zz_unpack(a, &cond, &stmt) != 2) {
                fprintf(stderr, "usage: while cond stmt: ");
                zz_print(a, stderr);
                fprintf(stderr, "\n");
                return NULL;
        }
        while (eval(env, cond))
                eval(env, stmt);
        return NULL;
}
struct zz_ast *op_for(struct array *env, struct zz_ast *a)
{
        struct zz_ast *var, *list, *stmt;
        if (zz_unpack(a, &var, &list, &stmt) != 3)
                goto fail;
        if (zz_typeof(var) != zz_sym_type())
                goto fail;
        list = eval(env, list);
        struct zz_ast *it;
        zz_foreach(it, list) {
                op_set(env, zz_list(var, it).first);
                eval(env, stmt);
        }
        return NULL;
 fail:
        fprintf(stderr, "usage: for var list stmt: ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
struct zz_ast *op_case(struct array *env, struct zz_ast *a)
{
        struct zz_ast *var;
        if (zz_unpack(a, &var) != 1)
                goto fail;
        struct zz_ast *it;
        zz_foreach(it, zz_tail(a)) {
                struct zz_ast *val, *stmt;
                if (zz_unpack(it, &val, &stmt) != 2)
                        goto fail;
                if (zz_cmp(var, eval(env, val)) == 0)
                        return eval(env, stmt);
        }
        return NULL;
 fail:
        fprintf(stderr, "usage: case var [(val stmt)...]: ");
        zz_print(a, stderr);
        fprintf(stderr, "\n");
        return NULL;
}
