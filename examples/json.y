%{
#include "test.h"

struct zz_ast *json_array(void)
{
        static struct zz_ast *a = NULL;
        return a ? a : (a = zz_str("$array"));
}
struct zz_ast *json_object(void)
{
        static struct zz_ast *a = NULL;
        return a ? a : (a = zz_str("$object"));
}

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
%token ','
%token ':'
%token '['
%token ']'
%token '{'
%token '}'
%token<ast> STRING
%token<ast> NUMBER

%type<ast> expr
%type<ast> array
%type<list> expr_list
%type<ast> object
%type<list> pair_list
%type<ast> pair

%%

input
        : expr {
                zz_print($1, stdout);
                printf("\n");
        }
        ;

expr
        : STRING
        | NUMBER
        | '[' array ']' { $$ = zz_pair(json_array(), $2); }
        | '{' object '}' { $$ = zz_pair(json_object(), $2); }
        ;

array
        : expr_list { $$ = $1.first; }
        | { $$ = NULL; }
        ;

expr_list
        : expr_list ',' expr { $$ = zz_append($1, $3); }
        | expr { $$ = zz_list($1); }
        ;

object
        : pair_list { $$ = $1.first; }
        | { $$ = NULL; }
        ;

pair_list
        : pair_list ',' pair { $$ = zz_append($1, $3); }
        | pair { $$ = zz_list($1); }
        ;

pair
        : STRING ':' expr { $$ = zz_list($1, $3).first; }
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
                 case '\n':
                        break;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        lvalp->ast = zz_int(c);
                        return NUMBER;
                 case '"':
                        array_clear(&buf);
                        for (;;) {
                                switch ((c = fgetc(f))) {
                                 case EOF:
                                        fprintf(stderr, "Unterminated string\n");
                                        abort();
                                 case '"':
                                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                                        return STRING;
                                }
                                array_push_back(&buf, &c);
                        }
                 default:
                        return c;
                }
        }
}

