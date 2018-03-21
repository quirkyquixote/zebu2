
#include "zebu.h"

const char *ZZ_PAIR = "pair";

void zz_ast_init(struct zz_ast* a)
{
        a->ptr = NULL;
        a->size = 0;
        a->alloc = 0;
}
void zz_ast_deinit(struct zz_ast *a)
{
        free(a->ptr);
}
void *zz_node_new(struct zz_ast* a, int size)
{
        if (a->size == a->alloc) {
                a->alloc = a->alloc ? a->alloc * 2 : 2;
                a->ptr = realloc(a->ptr, a->alloc * sizeof(*a->ptr));
        }
        return (a->ptr[a->size++] = malloc(size));
}
void zz_ast_gc(struct zz_ast *a, struct zz_node *root)
{
        void *w_begin[a->size];
        void *g_begin[a->size];
        void *b_begin[a->size];
        void **w_end = w_begin + a->size;
        void **g_end = g_begin;
        void **b_end = b_begin;
        memcpy(w_begin, a->ptr, a->size * sizeof(void *));
        for (void **i = w_begin; i != w_end;) {
                if (*i == root) {
                        *i = *(--w_end);
                        break;
                } else {
                        ++i;
                }
        }
        *(g_end++) = root;
        fprintf(stderr, "before (W/G/B): (%ld/%ld/%ld)\n",
                        w_end - w_begin, g_end - g_begin, b_end - b_begin);
        while (g_begin != g_end) {
                void *x = *(--g_end);
                if (zz_is_pair(x)) {
                        struct zz_pair *p = zz_pair(x);
                        for (void **i = w_begin; i != w_end;) {
                                if (*i == p->data || *i == p->next) {
                                        *(g_end++) = *i;
                                        *i = *(--w_end);
                                } else {
                                        ++i;
                                }
                        }
                }
                *(b_end++) = x;;
        }
        fprintf(stderr, "after (W/G/B): (%ld/%ld/%ld)\n",
                        w_end - w_begin, g_end - g_begin, b_end - b_begin);
        for (void **i = w_begin; i != w_end; ++i)
                free(*i);
        fprintf(stderr, "%ld nodes collected\n", w_end - w_begin);
        a->size = b_end - b_begin;
        memcpy(a->ptr, b_begin, a->size * sizeof(void *));
}

struct zz_node *zz_pair_new(struct zz_ast *a,
                struct zz_node *data, struct zz_node *next)
{
        struct zz_pair *n = zz_node_new(a, sizeof(*n));
        n->type = ZZ_PAIR;
        n->data = data;
        n->next = next;
        return (void *)n;
}
int zz_is_pair(struct zz_node *n)
{
        return n != NULL && n->type == ZZ_PAIR;
}
struct zz_pair *zz_pair(struct zz_node *n)
{
        return zz_is_pair(n) ? (void *)n : NULL;
}

struct zz_node *zz_atom_new_with_len(struct zz_ast *a,
                const char *type, const char *str, int len)
{
        struct zz_atom *n = zz_node_new(a, sizeof(*n) + len + 1);
        n->type = type;
        memcpy(n->str, str, len);
        n->str[len] = 0;
        return (void *)n;
}
struct zz_node *zz_atom_new(struct zz_ast *a, const char *type,
                const char *str)
{
        return zz_atom_new_with_len(a, type, str, strlen(str));
}
int zz_is_atom(struct zz_node *n)
{
        return n != NULL && n->type != ZZ_PAIR;
}
struct zz_atom *zz_atom(struct zz_node *n)
{
        return zz_is_atom(n) ? (void *)n : NULL;
}

struct zz_node *zz_list_append(struct zz_ast *a,
                struct zz_node *head, struct zz_node *x)
{
        if (head == NULL)
                return zz_pair_new(a, x, NULL);
        struct zz_pair *i = zz_pair(head);
        while (i->next)
                i = zz_pair(i->next);
        i->next = zz_pair_new(a, x, NULL);
        return head;
}
struct zz_node *zz_list_prepend(struct zz_ast *a,
                struct zz_node *head, struct zz_node *x)
{
        return zz_pair_new(a, x, head);
}

int zz_print(struct zz_node *n, FILE *f)
{
        if (n == NULL) {
                return fprintf(f, "()");
        } else if (n->type == ZZ_PAIR) {
                int ret = fprintf(f, "(");
                struct zz_node *x;
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
                return fprintf(f, "%s:\"%s\"", n->type, zz_atom(n)->str);
        }
}
