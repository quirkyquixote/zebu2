
#include "zebu2.h"

int serialize_pair(struct zz_ast *a, FILE *f)
{
        struct zz_pair *x = (void *)a;
        int ret = 0;
        if (x->head)
                ret += zz_print(x->head, f);
        if (x->tail) {
                fputc(' ', f);
                ret += x->tail->type->serialize(x->tail, f);
        }
        return ret;
}
int serialize_atom(struct zz_ast *a, FILE *f)
{
        struct zz_atom *x = (void *)a;
        return fprintf(f, "%s", x->str);
}

const struct zz_type *zz_pair_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_pair
        };
        return &type;
}
const struct zz_type *zz_atom_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_atom
        };
        return &type;
}

struct zz_ast *zz_pair(struct zz_ast *head, struct zz_ast *tail)
{
        struct zz_pair *n = GC_malloc(sizeof(*n));
        n->type = zz_pair_type();
        n->head = head;
        n->tail = tail;
        return (void *)n;
}
int zz_is_pair(struct zz_ast *n)
{
        return n != NULL && n->type == zz_pair_type();
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

struct zz_ast *zz_atom_with_len(const char *str, int len)
{
        struct zz_atom *n = GC_malloc_atomic(sizeof(*n) + len + 1);
        n->type = zz_atom_type();
        memcpy(n->str, str, len);
        n->str[len] = 0;
        return (void *)n;
}
struct zz_ast *zz_atom(const char *str)
{
        return zz_atom_with_len(str, strlen(str));
}
int zz_is_atom(struct zz_ast *n)
{
        return n != NULL && n->type == zz_atom_type();
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
        int ret = 0;
        if (n == NULL) {
                ret += fprintf(f, "()");
        } else if (zz_is_pair(n)) {
                fputc('(', f);
                ret += n->type->serialize(n, f);
                fputc(')', f);
        } else {
                ret += n->type->serialize(n, f);
        }
        return ret;
}
