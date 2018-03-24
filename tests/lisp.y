%{
#include "bits.h"

/*
 * These are the tokens that define node types in the AST
 */
const char *TOK_NUM = "num";
const char *TOK_SYM = "sym";
const char *TOK_STR = "set";
const char *TOK_QUO = "quo";

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
%token '\''
%token '('
%token ')'
%token <ast> ATOM
%type <ast> expr
%type <ast> list

%%

input
    : input '\n' line
    | line
    ;

line
    : expr {
        zz_print($1, stdout);
        fputc('\n', stdout);
        }
    | 
    ;

expr
   : '(' list ')' { $$ = $2; }
   | '\'' expr { $$ = zz_pair(zz_atom(TOK_QUO, ""), zz_pair($2, NULL)); }
   | ATOM { $$ = $1; }
   ;

list
   : expr list { $$ = zz_pair($1, $2); }
   | { $$ = NULL; }
   ;

%%

#include <ctype.h>

int yylex(const char **ptr)
{
        while (*(*ptr) == ' ' || *(*ptr) == '\t' || *(*ptr) == '\r')
                ++(*ptr);

        switch (*(*ptr)) {
         case 0:
                return 0;
         case '(':
         case ')':
         case '\n':
         case '\'':
                return *((*ptr)++);
         case '0'...'9':
                {
                        const char *begin = (*ptr);
                        do
                                ++(*ptr);
                        while (isdigit(*(*ptr)));
                        yylval.ast = zz_atom_with_len(TOK_NUM, begin, (*ptr) - begin);
                }
                return ATOM;
         case '"':
                {
                        const char *begin = ++(*ptr);
                        do
                                ++(*ptr);
                        while (*(*ptr) != '"' && *(*ptr) != '\0');
                        yylval.ast = zz_atom_with_len(TOK_NUM, begin, (*ptr) - begin);
                }
                ++(*ptr);
                return ATOM;
         default:
                {
                        const char *begin = (*ptr);
                        do
                                ++(*ptr);
                        while (!isspace(*(*ptr)) && *(*ptr) != '(' &&
                                *(*ptr) != ')' && *(*ptr) != '\0');
                        yylval.ast = zz_atom_with_len(TOK_NUM, begin, (*ptr) - begin);
                }
                return ATOM;
        }
}
