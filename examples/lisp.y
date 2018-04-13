%{
#include "test.h"
%}

%define api.pure full
%param {FILE *f}

%union {
        struct zz_ast* ast;
        struct zz_list list;
}

/*
 * Token and rule declarations
 */
%token '\n'
%token '\''
%token '('
%token ')'
%token<ast> ATOM

%type<ast> expr
%type<list> list

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
   : '(' list ')' { $$ = $2.first; }
   | '\'' expr { $$ = zz_list(zz_str("quote"), $2).first; }
   | ATOM
   ;

list
   : list expr { $$ = zz_append($1, $2); }
   | { $$ = zz_list_empty(); }
   ;

%%

#include <ctype.h>

int yylex(YYSTYPE *lvalp, FILE *f)
{
        static struct array buf = array(sizeof(char));

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
                        lvalp->ast = zz_int(c);
                        return ATOM;
                 case '"':
                        array_clear(&buf);
                        for (;;) {
                                switch ((c = fgetc(f))) {
                                 case EOF:
                                        fprintf(stderr, "Unterminated string\n");
                                        abort();
                                 case '"':
                                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                                        return ATOM;
                                }
                                array_push_back(&buf, &c);
                        }
                 default:
                        array_clear(&buf);
                        do {
                                array_push_back(&buf, &c);
                                c = fgetc(f);
                        } while (c != EOF && !isspace(c));
                        ungetc(c, f);
                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                        return ATOM;
                }
        }
}

