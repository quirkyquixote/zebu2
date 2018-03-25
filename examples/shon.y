%{
#include <ctype.h>

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
    : '=' { $$ = zz_str("set"); }
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
    : '>' { $$ = zz_str("gt"); }
    | '<' { $$ = zz_str("lt"); }
    | OP_EQ { $$ = zz_str("eq"); }
    | OP_NE { $$ = zz_str("ne"); }
    | OP_GE { $$ = zz_str("ge"); }
    | OP_LE { $$ = zz_str("le"); }
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
    : '+' { $$ = zz_str("add"); }
    | '-' { $$ = zz_str("sub"); }
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
    : '*' { $$ = zz_str("mul"); }
    | '/' { $$ = zz_str("div"); }
    | '%' { $$ = zz_str("mod"); }
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
    : '^' { $$ = zz_str("exp"); }
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
                        yylval.ast = zz_str_with_len(begin, *ptr - begin);
                }
                return ATOM;
         case '0'...'9':
                yylval.ast = zz_int(strtol(*ptr, (char **)ptr, 10));
                return ATOM;
         case '"':
                {
                        const char *begin = ++*ptr;
                        *ptr = strchr(*ptr, '"');
                        if (*ptr == NULL) {
                                fprintf(stderr, "Unterminated string\n");
                                abort();
                        }
                        yylval.ast = zz_str_with_len(begin, *ptr - begin);
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

