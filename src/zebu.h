
#ifndef _ZEBU_H
#define _ZEBU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *ZZ_PAIR;

struct zz_node {
        const char* type;
};

int zz_print(struct zz_node *n, FILE *f);

struct zz_ast {
        void *blobs;
};

void zz_ast_init(struct zz_ast* a);
void zz_ast_deinit(struct zz_ast *a);
void *zz_ast_alloc(struct zz_ast* a, int size);
void zz_ast_gc(struct zz_ast *a, struct zz_node *root);

struct zz_pair {
        const char *type;
        struct zz_node* data;
        struct zz_node* next;
};

struct zz_node *zz_pair_new(struct zz_ast *a, struct zz_node *data,
                struct zz_node *next);
int zz_is_pair(struct zz_node *n);
struct zz_pair *zz_pair(struct zz_node *n);

#define zz_foreach(_x, _head) \
for (struct zz_pair* _i = zz_pair(_head); \
                _x = _i ? _i->data : NULL, _i; \
                _i = zz_pair(_i->next))

struct zz_atom {
        const char *type;
        char str[];
};

struct zz_node *zz_atom_new_with_len(struct zz_ast *a, const char *type,
                const char *str, int len);
struct zz_node *zz_atom_new(struct zz_ast *a, const char *type,
                const char *str);
int zz_is_atom(struct zz_node *n);
struct zz_atom *zz_atom(struct zz_node *n);

struct zz_node *zz_list_append(struct zz_ast *a, struct zz_node *head, struct zz_node *x);
struct zz_node *zz_list_prepend(struct zz_ast *a, struct zz_node *head, struct zz_node *x);


#endif  // _ZEBU_H
