%{

#include "bits.h"

/*
 * These are the tokens that define node types in the AST
 */
const char *TOK_NUM = "num";
const char *TOK_ADD = "add";
const char *TOK_SUB = "sub";
const char *TOK_MUL = "mul";
const char *TOK_DIV = "div";
const char *TOK_EXP = "exp";
const char *TOK_NEG = "neg";

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
        $$ = zz_pair(zz_atom(TOK_ADD, ""),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '-'     { 
        $$ = zz_pair(zz_atom(TOK_SUB, ""),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '*'     { 
        $$ = zz_pair(zz_atom(TOK_MUL, ""),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '/'     { 
        $$ = zz_pair(zz_atom(TOK_DIV, ""),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp exp '^'     { 
        $$ = zz_pair(zz_atom(TOK_EXP, ""),
             zz_pair($1,
             zz_pair($2,
             NULL)));
        }
    | exp 'n'         {
        $$ = zz_pair(zz_atom(TOK_NEG, ""),
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
                        yylval.ast = zz_atom_with_len(TOK_NUM, begin, (*ptr) - begin);
                }
                return NUM;
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

