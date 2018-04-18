%{
#include "test.h"

struct zz_ast *conf_line(void)
{
        struct zz_ast *a = NULL;
        return a ? a : (a = zz_str("$line"));
}
struct zz_ast *conf_block(void)
{
        struct zz_ast *a = NULL;
        return a ? a : (a = zz_str("$block"));
}
struct zz_ast *conf_comment(void)
{
        struct zz_ast *a = NULL;
        return a ? a : (a = zz_str("$comment"));
}

%}

%define api.pure full
%param {FILE *f}
%error-verbose

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
%token<ast> COMMENT

%type<list> block_list
%type<ast> block
%type<list> line_list
%type<ast> line
%type<ast> expr

%%

input
        : line_list block_list {
                zz_print(zz_prepend($2, $1.first).first, stdout);
                printf("\n");
        }
        ;

block_list
        : block_list block { $$ = zz_append($1, $2); }
        | { $$ = zz_list_empty(); }
        ;

block
        : '[' STRING ']' separator line_list {
                $$ = zz_pair(conf_block(), zz_pair($2, $5.first)); }
        ;

line_list
        : line_list line { $$ = zz_append($1, $2); }
        | { $$ = zz_list_empty(); }
        ;

line
        : STRING '=' expr separator { $$ = zz_list(conf_line(), $1, $3).first; }
        | COMMENT separator { $$ = zz_list(conf_comment(), $1).first; }
        ;

expr
        : STRING
        | NUMBER
        ;

separator
        : separator '\n'
        |
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
                 case '\n':
                 case '[':
                 case ']':
                 case '=':
                        return c;
                 case '0'...'9':
                        ungetc(c, f);
                        fscanf(f, "%d", &c);
                        lvalp->ast = zz_int(c);
                        return NUMBER;
                 case '#':
                        array_clear(&buf);
                        c = fgetc(f);
                        while (c != '\n' && c != EOF) {
                                array_push_back(&buf, &c);
                                c = fgetc(f);
                        }
                        ungetc(c, f);
                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                        return COMMENT;
                 default:
                        array_clear(&buf);
                        while (c != '=' && c != ']' && c != '[' && c != '#'
                                        && c != '\n' && c != EOF) {
                                array_push_back(&buf, &c);
                                c = fgetc(f);
                        }
                        ungetc(c, f);
                        while (isspace(buf.buf[buf.len - 1]))
                                --buf.len;
                        lvalp->ast = zz_str_with_len(buf.buf, buf.len);
                        return STRING;
                }
        }
}

