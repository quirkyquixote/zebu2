
#include "test.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "gc.h"
#include "readline/readline.h"
#include "readline/history.h"

void *array_index(struct array *a, int i)
{
        return a->buf + i * a->siz;
}
void array_resize(struct array *a, int len)
{
        if (len > a->cap) {
                if (a->cap == 0)
                        a->cap = 2;
                while (len > a->cap)
                        a->cap *= 2;
                a->buf = GC_realloc(a->buf, a->cap * a->siz);
        }
        a->len = len;
}
void array_clear(struct array *a)
{
        array_resize(a, 0);
}
void array_replace(struct array *a, int i, void *p)
{
        assert(i >= 0 && i < a->len);
        memcpy(array_index(a, i), p, a->siz);
}
void array_insert(struct array *a, int i, void *p)
{
        assert(i >= 0 && i <= a->len);
        array_resize(a, a->len + 1);
        memmove(array_index(a, i + 1), array_index(a, i),
                        (a->len - i - 1) * a->siz);
        array_replace(a, i, p);
}
void array_erase(struct array *a, int i)
{
        assert(i >= 0 && i < a->len);
        memmove(array_index(a, i), array_index(a, i + 1),
                        (a->len - i - 1) * a->siz);
        array_resize(a, a->len - 1);
}
void *array_front(struct array *a)
{
        return array_index(a, 0);
}
void *array_back(struct array *a)
{
        return array_index(a, a->len - 1);
}
void array_push_front(struct array *a, void *p)
{
        array_insert(a, 0, p);
}
void array_push_back(struct array *a, void *p)
{
        array_insert(a, a->len, p);
}
void array_pop_front(struct array *a)
{
        array_erase(a, 0);
}
void array_pop_back(struct array *a)
{
        array_erase(a, a->len - 1);
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

