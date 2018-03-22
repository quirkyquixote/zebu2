
#include "zebu.h"

const char *ZZ_PAIR = "pair";

struct zz_blob *zz_blob_new(void)
{
        struct zz_blob *node = malloc(sizeof(*node));
        node->next = node;
        node->prev = node;
        return node;
}
void zz_blob_destroy(struct zz_blob *node)
{
        node->prev->next = NULL;
        while (node) {
                struct zz_blob *next = node->next;
                free(node);
                node = next;
        }
}
int zz_empty(struct zz_blob *node)
{
        return node->next == node;
}
void zz_link(struct zz_blob *node, struct zz_blob *next)
{
        struct zz_blob *prev = next->prev;
        node->next = next;
        node->prev = prev;
        prev->next = node;
        next->prev = node;
}
void zz_unlink(struct zz_blob *node)
{
        node->next->prev = node->prev;
        node->prev->next = node->next;
}

void zz_ast_init(struct zz_ast* a)
{
        a->blobs = zz_blob_new();
}
void zz_ast_deinit(struct zz_ast *a)
{
        zz_blob_destroy(a->blobs);
}
void *zz_node_new(struct zz_ast* a, int size)
{
        struct zz_blob *b = malloc(sizeof(*b) + size);
        zz_link(b, a->blobs);
        return (void *)(b + 1);
}
void zz_ast_gc(struct zz_ast *a, struct zz_node *root)
{
        if (zz_empty(a->blobs))
                return;

        struct zz_blob *white = a->blobs;
        struct zz_blob *gray = zz_blob_new();
        struct zz_blob *black = zz_blob_new();

        {
                struct zz_blob *b = (struct zz_blob *)root - 1;
                zz_unlink(b);
                zz_link(b, gray);
        }

        while (!zz_empty(gray)) {
                struct zz_blob *b = gray->next;
                zz_unlink(b);
                struct zz_node *n = (void *)(b + 1);
                if (zz_is_pair(n)) {
                        struct zz_pair *p = zz_pair(n);
                        if (p->data) {
                                struct zz_blob *c = (struct zz_blob *)p->data - 1;
                                zz_unlink(c);
                                zz_link(c, gray);
                        }
                        if (p->next) {
                                struct zz_blob *c = (struct zz_blob *)p->next - 1;
                                zz_unlink(c);
                                zz_link(c, gray);
                        }
                }
                zz_link(b, black);
        }

        zz_blob_destroy(white);
        zz_blob_destroy(gray);
        a->blobs = black;
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
