
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
                if (size == alloc)
                        buf = GC_realloc(buf, alloc *= 2);
        }
        buf[size++] = 0;
        return buf;
}

struct zz_ast *prune(struct zz_ast *a, int first)
{
        struct zz_pair *p = zz_cast_or_null(zz_pair, a);
        if (p == NULL)
                return a;
        if (first && p->tail == NULL && zz_cast_or_null(zz_pair, p->head))
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
                        yyparse(f);
                        fclose(f);
                }
        } else if (isatty(fileno(stdin))) {
                for (;;) {
                        char *line = readline("$ ");
                        if (line) {
                                if (*line) {
                                        add_history(line);
                                        FILE *f = tmpfile();
                                        fwrite(line, strlen(line), 1, f);
                                        fseek(f, SEEK_SET, 0);
                                        yyparse(f);
                                        fclose(f);
                                }
                                free(line);
                        }
                }
        } else {
                yyparse(stdin);
        }
}

