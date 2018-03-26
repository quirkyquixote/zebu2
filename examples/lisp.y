%{
#include "test.h"
%}

%define api.pure full
%param {const char **ptr}
%define api.value.type {struct zz_ast *}

/*
 * Token and rule declarations
 */
%token '\n'
%token '\''
%token '('
%token ')'
%token ATOM

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
   | '\'' expr { $$ = zz_pair(zz_str("quote"), zz_pair($2, NULL)); }
   | ATOM { $$ = $1; }
   ;

list
   : expr list { $$ = zz_pair($1, $2); }
   | { $$ = NULL; }
   ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, const char **ptr)
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
                *lvalp = zz_int(strtol(*ptr, (char **)ptr, 10));
                return ATOM;
         case '"':
                {
                        const char *begin = ++*ptr;
                        *ptr = strchr(*ptr, '"');
                        if (*ptr == NULL) {
                                fprintf(stderr, "Unterminated string\n");
                                abort();
                        }
                        *lvalp = zz_str_with_len(begin, *ptr - begin);
                        ++*ptr;
                }
                return ATOM;
         default:
                {
                        const char *begin = (*ptr);
                        do
                                ++(*ptr);
                        while (!isspace(*(*ptr)) && *(*ptr) != '(' &&
                                *(*ptr) != ')' && *(*ptr) != '\0');
                        *lvalp = zz_str_with_len(begin, (*ptr) - begin);
                }
                return ATOM;
        }
}

