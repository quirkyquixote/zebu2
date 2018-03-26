
#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "gc.h"
#include "readline/readline.h"
#include "readline/history.h"

char *slurp(FILE *f)
{
        int size = 0;
        int alloc = 2;
        char *buf = GC_malloc_atomic(alloc);
        int c;
        while ((c = fgetc(f)) != EOF) {
                buf[size++] = c;
                if (size == alloc) {
                        alloc = alloc ? alloc * 2 : 2;
                        buf = GC_realloc(buf, alloc);
                }
        }
        buf[size++] = 0;
        return buf;
}

struct zz_ast *prune(struct zz_ast *a, int first)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        if (p == NULL)
                return a;
        if (first && p->tail == NULL && zz_cast(zz_pair, p->head))
                return prune(p->head, 1);
        p->head = prune(p->head, 1);
        p->tail = prune(p->tail, 0);
        return a;
}

void yyerror(const char **ptr, const char *msg)
{
        fprintf(stdout, "%s\n", msg);
}

int main(int argc, char *argv[])
{
        GC_init();

        if (argc > 1) {
                for (int i = 1; i < argc; ++i) {
                        FILE *f = fopen(argv[i], "r");
                        if (f == NULL) {
                                perror(argv[i]);
                                continue;
                        }
                        char *buf = slurp(f);
                        fclose(f);
                        const char *ptr = buf;
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
                char *buf = slurp(stdin);
                const char *ptr = buf;
                yyparse(&ptr);
        }
}

