
#ifndef _ZEBU_H
#define _ZEBU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *ZZ_PAIR;

struct zz_ast {
        const char* type;
};

int zz_print(struct zz_ast *n, FILE *f);

struct zz_ast_mgr {
        void *blobs;
};

void zz_ast_mgr_init(struct zz_ast_mgr* a);
void zz_ast_mgr_deinit(struct zz_ast_mgr *a);
void *zz_ast_mgr_alloc(struct zz_ast_mgr* a, int size);
void zz_ast_mgr_gc(struct zz_ast_mgr *a, struct zz_ast *root);

struct zz_pair {
        const char *type;
        struct zz_ast* data;
        struct zz_ast* next;
};

struct zz_ast *zz_pair(struct zz_ast_mgr *a, struct zz_ast *data,
                struct zz_ast *next);
int zz_is_pair(struct zz_ast *n);
struct zz_pair *zz_to_pair(struct zz_ast *n);

#define zz_foreach(_x, _head) \
for (struct zz_pair* _i = zz_to_pair(_head); \
                _x = _i ? _i->data : NULL, _i; \
                _i = zz_to_pair(_i->next))

struct zz_atom {
        const char *type;
        char str[];
};

struct zz_ast *zz_atom_with_len(struct zz_ast_mgr *a, const char *type,
                const char *str, int len);
struct zz_ast *zz_atom(struct zz_ast_mgr *a, const char *type,
                const char *str);
int zz_is_atom(struct zz_ast *n);
struct zz_atom *zz_to_atom(struct zz_ast *n);

struct zz_ast *zz_list_append(struct zz_ast_mgr *a, struct zz_ast *head, struct zz_ast *x);
struct zz_ast *zz_list_prepend(struct zz_ast_mgr *a, struct zz_ast *head, struct zz_ast *x);


#endif  // _ZEBU_H
