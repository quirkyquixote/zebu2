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
        fputc('\n', stdout);
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
    : '=' { $$ = zz_atom("set"); }
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
    : '+' { $$ = zz_atom("add"); }
    | '-' { $$ = zz_atom("sub"); }
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
    : '*' { $$ = zz_atom("mul"); }
    | '/' { $$ = zz_atom("div"); }
    | '%' { $$ = zz_atom("mod"); }
    | '^' { $$ = zz_atom("exp"); }
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
                        yylval.ast = zz_atom_with_len(begin, (*ptr) - begin);
                }
                return ATOM;
         case '0'...'9':
                {
                        const char *begin = (*ptr);
                        do
                                ++(*ptr);
                        while (isdigit(*(*ptr)));
                        yylval.ast = zz_atom_with_len(begin, (*ptr) - begin);
                }
                return ATOM;
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

