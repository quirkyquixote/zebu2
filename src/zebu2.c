
#include "zebu2.h"

const char *ZZ_PAIR = "pair";

struct zz_blob {
        struct zz_blob *next, *prev;
};

struct zz_blob *zz_blob(int size)
{
        struct zz_blob *node = malloc(sizeof(*node) + size);
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
int zz_blob_empty(struct zz_blob *node)
{
        return node->next == node;
}
void zz_blob_link(struct zz_blob *node, struct zz_blob *next)
{
        struct zz_blob *prev = next->prev;
        node->next = next;
        node->prev = prev;
        prev->next = node;
        next->prev = node;
}
void zz_blob_unlink(struct zz_blob *node)
{
        node->next->prev = node->prev;
        node->prev->next = node->next;
}

void zz_ast_mgr_init(struct zz_ast_mgr* a)
{
        a->blobs = zz_blob(0);
}
void zz_ast_mgr_deinit(struct zz_ast_mgr *a)
{
        zz_blob_destroy(a->blobs);
}
void *zz_ast_mgr_alloc(struct zz_ast_mgr* a, int size)
{
        struct zz_blob *b = zz_blob(size);
        zz_blob_link(b, a->blobs);
        return (void *)(b + 1);
}
void zz_ast_mgr_gc(struct zz_ast_mgr *a, struct zz_ast *root)
{
        if (zz_blob_empty(a->blobs))
                return;

        struct zz_blob *white = a->blobs;
        struct zz_blob *gray = zz_blob(0);
        struct zz_blob *black = zz_blob(0);

        {
                struct zz_blob *b = (struct zz_blob *)root - 1;
                zz_blob_unlink(b);
                zz_blob_link(b, gray);
        }

        while (!zz_blob_empty(gray)) {
                struct zz_blob *b = gray->next;
                zz_blob_unlink(b);
                struct zz_ast *n = (void *)(b + 1);
                if (zz_is_pair(n)) {
                        struct zz_pair *p = zz_to_pair(n);
                        if (p->data) {
                                struct zz_blob *c = (struct zz_blob *)p->data - 1;
                                zz_blob_unlink(c);
                                zz_blob_link(c, gray);
                        }
                        if (p->next) {
                                struct zz_blob *c = (struct zz_blob *)p->next - 1;
                                zz_blob_unlink(c);
                                zz_blob_link(c, gray);
                        }
                }
                zz_blob_link(b, black);
        }

        zz_blob_destroy(white);
        zz_blob_destroy(gray);
        a->blobs = black;
}

struct zz_ast *zz_pair(struct zz_ast_mgr *a, struct zz_ast *data,
                struct zz_ast *next)
{
        struct zz_pair *n = zz_ast_mgr_alloc(a, sizeof(*n));
        n->type = ZZ_PAIR;
        n->data = data;
        n->next = next;
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

struct zz_ast *zz_atom_with_len(struct zz_ast_mgr *a, const char *type,
                const char *str, int len)
{
        struct zz_atom *n = zz_ast_mgr_alloc(a, sizeof(*n) + len + 1);
        n->type = type;
        memcpy(n->str, str, len);
        n->str[len] = 0;
        return (void *)n;
}
struct zz_ast *zz_atom(struct zz_ast_mgr *a, const char *type, const char *str)
{
        return zz_atom_with_len(a, type, str, strlen(str));
}
int zz_is_atom(struct zz_ast *n)
{
        return n != NULL && n->type != ZZ_PAIR;
}
struct zz_atom *zz_to_atom(struct zz_ast *n)
{
        return zz_is_atom(n) ? (void *)n : NULL;
}

struct zz_ast *zz_list_append(struct zz_ast_mgr *a,
                struct zz_ast *head, struct zz_ast *x)
{
        if (head == NULL)
                return zz_pair(a, x, NULL);
        struct zz_pair *i = zz_to_pair(head);
        while (i->next)
                i = zz_to_pair(i->next);
        i->next = zz_pair(a, x, NULL);
        return head;
}
struct zz_ast *zz_list_prepend(struct zz_ast_mgr *a,
                struct zz_ast *head, struct zz_ast *x)
{
        return zz_pair(a, x, head);
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
