%{
#include "test.h"
%}

%define api.pure full
%param {FILE *f}
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

int yylex(YYSTYPE *lvalp, FILE *f)
{
        static int size = 0;
        static int alloc = 0;
        static char *buf = NULL;

        for (;;) {
                int c = fgetc(f);
                switch (c) {
                 case EOF:
                        return 0;
                 case ' ':
                 case '\t':
                 case '\r':
                        break;
                 case '(':
                 case ')':
                 case '\n':
                 case '\'':
                        return c;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        *lvalp = zz_int(c);
                        return ATOM;
                 case '"':
                        size = 0;
                        for (;;) {
                                switch ((c = fgetc(f))) {
                                 case EOF:
                                        fprintf(stderr, "Unterminated string\n");
                                        abort();
                                 case '"':
                                        *lvalp = zz_str_with_len(buf, size);
                                        return ATOM;
                                }
                                if (size == alloc) {
                                        alloc = alloc ? alloc * 2 : 2;
                                        buf = realloc(buf, alloc);
                                }
                                buf[size++] = c;
                        }
                 default:
                        size = 0;
                        do {
                                if (size == alloc) {
                                        alloc = alloc ? alloc * 2 : 2;
                                        buf = realloc(buf, alloc);
                                }
                                buf[size++] = c;
                                c = fgetc(f);
                        } while (c != EOF && !isspace(c));
                        ungetc(c, f);
                        *lvalp = zz_str_with_len(buf, size);
                        return ATOM;
                }
        }
}

