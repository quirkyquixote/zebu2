%{
#include "bits.h"

int yylex(const char **ptr);

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
%token '='
%token '+'
%token '-'
%token '*'
%token '/'
%token '%'
%token '('
%token ')'
%token <ast> ATOM
%type <ast> expression
%type <ast> assignment_expression
%type <ast> assignment_operator
%type <ast> additive_expression
%type <ast> additive_operator
%type <ast> multiplicative_expression
%type <ast> multiplicative_operator
%type <ast> atomic_expression

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
    : additive_expression assignment_operator assignment_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | additive_expression {
        $$ = $1;
        }
    ;

assignment_operator
    : '=' { $$ = zz_ptr(op_set); }
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
    : '+' { $$ = zz_ptr(op_add); }
    | '-' { $$ = zz_ptr(op_sub); }
    ;

multiplicative_expression
    : atomic_expression multiplicative_operator multiplicative_expression {
        $$ = zz_pair($2, zz_pair($1, zz_pair($3, NULL)));
        }
    | atomic_expression {
        $$ = $1;
        }
    ;

multiplicative_operator
    : '*' { $$ = zz_ptr(op_mul); }
    | '/' { $$ = zz_ptr(op_div); }
    | '%' { $$ = zz_ptr(op_mod); }
    | '^' { $$ = zz_ptr(op_exp); }
    ;

atomic_expression
    : ATOM { $$ = $1; }
    | '(' expression ')' { $$ = $2; }
    ;

%%

#include <ctype.h>

int yylex(const char **ptr)
{
        while (*(*ptr) == ' ' || *(*ptr) == '\t' || *(*ptr) == '\r')
                ++(*ptr);

        switch (*(*ptr)) {
         case 'a'...'z':
         case 'A'...'Z':
         case '_':
                {
                        const char *begin = (*ptr);
                        do
                                ++(*ptr);
                        while (isalnum(*(*ptr)) || *(*ptr) == '_');
                        yylval.ast = zz_str_with_len(begin, (*ptr) - begin);
                }
                return ATOM;
         case '0'...'9':
                yylval.ast = zz_int(strtol(*ptr, (char**)ptr, 10));
                return ATOM;
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

