%{

#include "bits.h"

int yylex(const char **ptr);

typedef int (operator)(struct zz_ast*);

int eval(struct zz_ast *a)
{
        if (zz_is_int(a)) {
                return zz_to_int(a)->num;
        } else if (zz_is_pair(a)) {
                operator *op = zz_to_ptr(zz_head(a))->ptr;
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

%param {const char **ptr}
/*
 * All data is represented by nodes.
 */
%union {
    struct zz_ast *ast;
}

/*
 * Token and rule declarations
 */
%token '\n'
%token '+'
%token '-'
%token '*'
%token '/'
%token 'n'
%token <ast> NUM
%type <ast> exp

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

int yylex(const char **ptr)
{
        while (*(*ptr) == ' ' || *(*ptr) == '\t' || *(*ptr) == '\r')
                ++(*ptr);

        switch (*(*ptr)) {
         case '0'...'9':
                yylval.ast = zz_int(strtol(*ptr, (char **)ptr, 10));
                return NUM;
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

