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
%token '+'
%token '-'
%token '*'
%token '/'
%token 'n'
%token NUM

%%

input
    : input '\n' line
    | line
    ;

line
    : exp {
        zz_print($1, stdout);
        fprintf(stdout, " = %d\n", eval($1));
        }
    | 
    ;

exp
    : NUM {
        $$ = $1;
        }
    | exp exp '+' { 
        $$ = zz_pair(zz_ptr(op_add), zz_pair($1, zz_pair($2, NULL)));
        }
    | exp exp '-' { 
        $$ = zz_pair(zz_ptr(op_sub), zz_pair($1, zz_pair($2, NULL)));
        }
    | exp exp '*' { 
        $$ = zz_pair(zz_ptr(op_mul), zz_pair($1, zz_pair($2, NULL)));
        }
    | exp exp '/' { 
        $$ = zz_pair(zz_ptr(op_div), zz_pair($1, zz_pair($2, NULL)));
        }
    | exp exp '^' { 
        $$ = zz_pair(zz_ptr(op_exp), zz_pair($1, zz_pair($2, NULL)));
        }
    | exp 'n' {
        $$ = zz_pair(zz_ptr(op_neg), zz_pair($1, NULL));
        }
    ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, FILE *f)
{
        for (;;) {
                int c = fgetc(f);
                switch (c) {
                 case EOF:
                        return 0;
                 case ' ':
                 case '\t':
                 case '\r':
                        break;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        *lvalp = zz_int(c);
                        return NUM;
                 default:
                        return c;
                }
        }
}

