%{

#include "bits.h"

int yylex(const char **ptr);

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
        fputc('\n', stdout);
        }
    | 
    ;

exp
    : NUM {
        $$ = $1;
        }
    | exp exp '+'     { 
        $$ = zz_pair(zz_atom("add"),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '-'     { 
        $$ = zz_pair(zz_atom("sub"),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '*'     { 
        $$ = zz_pair(zz_atom("mul"),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '/'     { 
        $$ = zz_pair(zz_atom("div"),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '^'     { 
        $$ = zz_pair(zz_atom("exp"),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp 'n'         {
        $$ = zz_pair(zz_atom("neg"),
             zz_pair($1,
             NULL));
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
                {
                        const char *begin = (*ptr)++;
                        while (isdigit(*(*ptr)))
                                ++(*ptr);
                        yylval.ast = zz_atom_with_len(begin, (*ptr) - begin);
                }
                return NUM;
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

