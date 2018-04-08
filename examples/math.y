%{
#include "test.h"

typedef int (operator)(struct zz_ast*);

int eval(struct zz_ast *a)
{
        if (a->type == zz_int_type()) {
                return zz_cast(zz_int, a)->num;
        } else if (a->type == zz_pair_type()) {
                operator *op = zz_cast(zz_ptr, zz_head(a))->ptr;
                return op(zz_tail(a));
        } else {
                abort();
        }
}

int op_set(struct zz_ast *a)
{
        return eval(zz_head(zz_tail(a)));
}
int op_add(struct zz_ast *a)
{
        return eval(zz_head(a)) + eval(zz_head(zz_tail(a)));
}
int op_sub(struct zz_ast *a)
{
        return eval(zz_head(a)) - eval(zz_head(zz_tail(a)));
}
int op_mul(struct zz_ast *a)
{
        return eval(zz_head(a)) * eval(zz_head(zz_tail(a)));
}
int op_div(struct zz_ast *a)
{
        return eval(zz_head(a)) / eval(zz_head(zz_tail(a)));
}
int op_mod(struct zz_ast *a)
{
        return eval(zz_head(a)) % eval(zz_head(zz_tail(a)));
}
int op_exp(struct zz_ast *a)
{
        return eval(zz_head(a)) * eval(zz_head(zz_tail(a)));
}
int op_neg(struct zz_ast *a)
{
        return -eval(zz_head(a));
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
%token ATOM

%%

input
    : input '\n' line
    | line
    ;

line
    : expression {
        zz_print($1, stdout);
        fprintf(stdout, " = %d\n", eval($1));
        }
    |
    ;

expression
   : assignment_expression { $$ = $1; }
   ;

assignment_expression
    : assignment_expression assignment_operator additive_expression {
        $$ = zz_list($2, $1, $3).first;
        }
    | additive_expression
    ;

assignment_operator
    : '=' { $$ = zz_ptr(op_set); }
    ;

additive_expression
    : additive_expression additive_operator multiplicative_expression {
        $$ = zz_list($2, $1, $3).first;
        }
    | multiplicative_expression
    ;

additive_operator
    : '+' { $$ = zz_ptr(op_add); }
    | '-' { $$ = zz_ptr(op_sub); }
    ;

multiplicative_expression
    : multiplicative_expression multiplicative_operator primary_expression {
        $$ = zz_list($2, $1, $3).first;
        }
    | primary_expression
    ;

multiplicative_operator
    : '*' { $$ = zz_ptr(op_mul); }
    | '/' { $$ = zz_ptr(op_div); }
    | '%' { $$ = zz_ptr(op_mod); }
    | '^' { $$ = zz_ptr(op_exp); }
    ;

primary_expression
    : ATOM { $$ = $1; }
    | '(' expression ')' { $$ = $2; }
    ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, FILE *f)
{
        static int size = 0;
        static int alloc = 0;
        static char *buf = NULL;

        for (;;) {
                int c = fgetc(f);
                switch (c) {
                 case EOF:
                        return 0;
                 case ' ':
                 case '\t':
                 case '\r':
                        break;
                 case 'a'...'z':
                 case 'A'...'Z':
                 case '_':
                        size = 0;
                        do {
                                if (size == alloc) {
                                        alloc = alloc ? alloc * 2 : 2;
                                        buf = realloc(buf, alloc);
                                }
                                buf[size++] = c;
                                c = fgetc(f);
                        } while (isalnum(c) || c == '_');
                        ungetc(c, f);
                        *lvalp = zz_str_with_len(buf, size);
                        return ATOM;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        *lvalp = zz_int(c);
                        return ATOM;
                 default:
                        return c;
                }
        }
}

