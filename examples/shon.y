%{
#include <ctype.h>

#include "bits.h"

/*
 * These are the tokens that define node types in the AST
 */
const char *TOK_NUM = "num";
const char *TOK_SYM = "sym";
const char *TOK_STR = "str";

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
%token '<'
%token '>'
%token OP_EQ
%token OP_NE
%token OP_GE
%token OP_LE
%token <ast> ATOM

%type <ast> statement_list
%type <ast> statement
%type <ast> expression
%type <ast> assignment_expression
%type <ast> assignment_operator
%type <ast> comparative_expression
%type <ast> comparative_operator
%type <ast> additive_expression
%type <ast> additive_operator
%type <ast> multiplicative_expression
%type <ast> multiplicative_operator
%type <ast> exponential_expression
%type <ast> exponential_operator
%type <ast> atomic_expression

%%

input
    : input '\n' line
    | line
    ;

line
    : statement_list {
        zz_print(prune($1, 1), stdout);
        fputc('\n', stdout);
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
    : '=' { $$ = zz_atom(TOK_SYM, "set"); }
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
    : '>' { $$ = zz_atom(TOK_SYM, "gt"); }
    | '<' { $$ = zz_atom(TOK_SYM, "lt"); }
    | OP_EQ { $$ = zz_atom(TOK_SYM, "eq"); }
    | OP_NE { $$ = zz_atom(TOK_SYM, "ne"); }
    | OP_GE { $$ = zz_atom(TOK_SYM, "ge"); }
    | OP_LE { $$ = zz_atom(TOK_SYM, "le"); }
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
    : '+' { $$ = zz_atom(TOK_SYM, "add"); }
    | '-' { $$ = zz_atom(TOK_SYM, "sub"); }
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
    : '*' { $$ = zz_atom(TOK_SYM, "mul"); }
    | '/' { $$ = zz_atom(TOK_SYM, "div"); }
    | '%' { $$ = zz_atom(TOK_SYM, "mod"); }
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
    : '^' { $$ = zz_atom(TOK_SYM, "exp"); }
    ;

atomic_expression
    : ATOM { $$ = $1; }
    | '(' statement_list ')' { $$ = $2; }
    ;

%%

int yylex(const char **ptr)
{
        while (**ptr == ' ' || **ptr == '\t' || **ptr == '\r')
                ++*ptr;

        switch (**ptr) {
         case 'a'...'z':
         case 'A'...'Z':
         case '_':
                {
                        const char *begin = (*ptr)++;
                        while (isalnum(**ptr) || **ptr == '_')
                                ++*ptr;
                        yylval.ast = zz_atom_with_len(TOK_SYM, begin, *ptr - begin);
                }
                return ATOM;
         case '0'...'9':
                {
                        const char *begin = (*ptr)++;
                        while (isdigit(**ptr))
                                ++*ptr;
                        yylval.ast = zz_atom_with_len(TOK_NUM, begin, *ptr - begin);
                }
                return ATOM;
         case '"':
                {
                        const char *begin = ++*ptr;
                        while (**ptr != '"' && **ptr != 0)
                                ++*ptr;
                        yylval.ast = zz_atom_with_len(TOK_STR, begin, *ptr - begin);
                        ++*ptr;
                }
                return ATOM;
         case '=':
                if (*++*ptr == '=') {
                        ++*ptr;
                        return OP_EQ;
                }
                return '=';
         case '>':
                if (*++*ptr == '=') {
                        ++*ptr;
                        return OP_GE;
                }
                return '>';
         case '<':
                if (*++*ptr == '=') {
                        ++*ptr;
                        return OP_LE;
                }
                return '<';
         case '!':
                if (*++*ptr == '=') {
                        ++*ptr;
                        return OP_NE;
                }
                return '!';
         case 0:
                return 0;
         default:
                return *((*ptr)++);
        }
}

