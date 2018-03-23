
#include "zebu2.h"

#include <gc.h>

const char *ZZ_PAIR = "pair";

struct zz_ast *zz_pair(struct zz_ast *head, struct zz_ast *tail)
{
        struct zz_pair *n = GC_malloc(sizeof(*n));
        n->type = ZZ_PAIR;
        n->head = head;
        n->tail = tail;
        return (void *)n;
}
int zz_is_pair(struct zz_ast *n)
{
        return n != NULL && n->type == ZZ_PAIR;
}
struct zz_pair *zz_to_pair(struct zz_ast *n)
{
        return zz_is_pair(n) ? (void *)n : NULL;
}
struct zz_ast* zz_head(struct zz_ast* a)
{
        struct zz_pair *p = zz_to_pair(a);
        return p == NULL ? a : p->head;
}
struct zz_ast* zz_tail(struct zz_ast* a)
{
        struct zz_pair *p = zz_to_pair(a);
        return p == NULL ? NULL : p->tail;
}

struct zz_ast *zz_atom_with_len(const char *type, const char *str, int len)
{
        struct zz_atom *n = GC_malloc_atomic(sizeof(*n) + len + 1);
        n->type = type;
        memcpy(n->str, str, len);
        n->str[len] = 0;
        return (void *)n;
}
struct zz_ast *zz_atom(const char *type, const char *str)
{
        return zz_atom_with_len(type, str, strlen(str));
}
int zz_is_atom(struct zz_ast *n)
{
        return n != NULL && n->type != ZZ_PAIR;
}
struct zz_atom *zz_to_atom(struct zz_ast *n)
{
        return zz_is_atom(n) ? (void *)n : NULL;
}

struct zz_ast *zz_list_append(struct zz_ast *head, struct zz_ast *x)
{
        if (head == NULL)
                return zz_pair(x, NULL);
        struct zz_pair *i = zz_to_pair(head);
        while (i->tail)
                i = zz_to_pair(i->tail);
        i->tail = zz_pair(x, NULL);
        return head;
}
struct zz_ast *zz_list_prepend(struct zz_ast *head, struct zz_ast *x)
{
        return zz_pair(x, head);
}

int zz_print(struct zz_ast *n, FILE *f)
{
        if (n == NULL) {
                return fprintf(f, "()");
        } else if (n->type == ZZ_PAIR) {
                int ret = fprintf(f, "(");
                struct zz_ast *x;
                int first = 1;
                zz_foreach(x, n) {
                        if (first)
                                first = 0;
                        else
                                ret += fprintf(f, " ");
                        ret += zz_print(x, f);
                }
                return ret + fprintf(f, ")");
        } else {
                return fprintf(f, "%s:\"%s\"", n->type, zz_to_atom(n)->str);
        }
}
