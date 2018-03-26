
#ifndef ZEBU_TEST_BITS_H_
#define ZEBU_TEST_BITS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "gc.h"
#include "readline/readline.h"
#include "readline/history.h"

#include "zebu2.h"

static char *prog_ptr;

struct strbuf {
        char *str;
        int size;
        int alloc;
};

void strbuf_clear(struct strbuf *b)
{
        b->size = 0;
}
void strbuf_append(struct strbuf *b, char c)
{
        if (b->size == b->alloc) {
                b->alloc = b->alloc ? b->alloc * 2 : 2;
                b->str = realloc(b->str, b->alloc);
        }
        b->str[b->size++] = c;
}
void strbuf_slurp(struct strbuf *b, FILE *f)
{
        int c;
        while ((c = fgetc(f)) != EOF)
                strbuf_append(b, c);
}

struct zz_ast* prune(struct zz_ast* a, int first)
{
        struct zz_pair* p = zz_cast(zz_pair, a);
        if (p == NULL)
                return a;
        if (first && p->tail == NULL && zz_cast(zz_pair, p->head))
                return prune(p->head, 1);
        p->head = prune(p->head, 1);
        p->tail = prune(p->tail, 0);
        return a;
}

int yyparse(const char **ptr);

void yyerror(const char **ptr, const char *msg)
{
        fprintf(stdout, "%s\n", msg);
}

int main(int argc, char *argv[])
{
        GC_init();

        if (argc > 1) {
                struct strbuf buf = {NULL, 0, 0};
                for (int i = 1; i < argc; ++i) {
                        strbuf_clear(&buf);
                        FILE *f = fopen(argv[i], "r");
                        if (f == NULL) {
                                perror(argv[i]);
                                continue;
                        }
                        strbuf_slurp(&buf, f);
                        fclose(f);
                        const char *ptr = buf.str;
                        yyparse(&ptr);
                }
        } else if (isatty(fileno(stdin))) {
                for (;;) {
                        char *line = readline("$ ");
                        if (line) {
                                if (*line) {
                                        add_history(line);
                                        const char *ptr = line;
                                        yyparse(&ptr);
                                }
                                free(line);
                        }
                }
        } else {
                struct strbuf buf = {NULL, 0, 0};
                strbuf_slurp(&buf, stdin);
                const char *ptr = buf.str;
                yyparse(&ptr);
        }
}

#endif  // ZEBU_TEST_BITS_H_
