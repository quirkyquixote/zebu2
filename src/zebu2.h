
#ifndef _ZEBU_H
#define _ZEBU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc.h>

struct zz_ast;

struct zz_type {
        const char *name;
        int (*serialize) (struct zz_ast *, FILE *);
        struct zz_ast *(* copy)(struct zz_ast *);
        int (* cmp)(struct zz_ast *, struct zz_ast *);
};

const struct zz_type *zz_null_type(void);
const struct zz_type *zz_pair_type(void);
const struct zz_type *zz_int_type(void);
const struct zz_type *zz_double_type(void);
const struct zz_type *zz_ptr_type(void);
const struct zz_type *zz_str_type(void);

struct zz_ast {
        const struct zz_type *type;
};

static inline const struct zz_type *zz_typeof(struct zz_ast *a)
{
        return a == NULL ? zz_null_type() : a->type;
}

static inline void *_zz_cast(const struct zz_type *t, struct zz_ast *a)
{
        if (zz_typeof(a) != t) {
                fprintf(stderr, "fatal: bad zz_cast. expected %s, got %s\n",
                                t->name, zz_typeof(a)->name);
                abort();
        }
        return a;
}

static inline void *_zz_cast_or_null(const struct zz_type *t, struct zz_ast *a)
{
        return zz_typeof(a) == t ? a : NULL;
}

#define zz_cast(_t, _a) ((struct _t*)_zz_cast(_t##_type(), _a))
#define zz_cast_or_null(_t, _a) ((struct _t*)_zz_cast_or_null(_t##_type(), _a))
int zz_print(struct zz_ast *n, FILE * f);
struct zz_ast *zz_copy(struct zz_ast *a);
int zz_cmp(struct zz_ast *a, struct zz_ast *b);

struct zz_pair {
        const struct zz_type *type;
        struct zz_ast *head;
        struct zz_ast *tail;
};

struct zz_ast *zz_pair(struct zz_ast *head, struct zz_ast *tail);
struct zz_ast *zz_head(struct zz_ast *a);
struct zz_ast *zz_tail(struct zz_ast *a);
void zz_set_head(struct zz_ast *a, struct zz_ast *head);
void zz_set_tail(struct zz_ast *a, struct zz_ast *tail);
struct zz_ast *zz_insert(struct zz_ast *a, struct zz_ast *next);
int _zz_unpack(struct zz_ast *list, ...);
#define zz_unpack(...) _zz_unpack(__VA_ARGS__, zz_arg_list_end())
struct zz_ast *zz_index(struct zz_ast *a, int i);

struct zz_ast *zz_find(struct zz_ast *l, struct zz_ast *x);
struct zz_ast *zz_find_if(struct zz_ast *l,
                int(* func)(struct zz_ast *, void *), void *data);
struct zz_ast *zz_remove(struct zz_ast *l, struct zz_ast *x);
struct zz_ast *zz_remove_if(struct zz_ast *l,
                int(* func)(struct zz_ast *, void *), void *data);
struct zz_ast *zz_sort(struct zz_ast *l,
                int(* comp)(struct zz_ast *, struct zz_ast *));

#define zz_foreach(_x, _head) \
for (struct zz_ast* _i = _head; _x = zz_head(_i), _i; _i = zz_tail(_i))

struct zz_ast *zz_null(void);

struct zz_int {
        const struct zz_type *type;
        int num;
};

struct zz_ast *zz_int(int num);

struct zz_double {
        const struct zz_type *type;
        double num;
};

struct zz_ast *zz_double(double num);

struct zz_ptr {
        const struct zz_type *type;
        void *ptr;
};

struct zz_ast *zz_ptr(void *ptr);

struct zz_str {
        const struct zz_type *type;
        char str[];
};

struct zz_ast *zz_str_with_len(const char *str, int len);
struct zz_ast *zz_str(const char *str);

struct zz_list {
        struct zz_ast *first;
        struct zz_ast *last;
};

struct zz_list zz_list_empty(void);
struct zz_list _zz_list(struct zz_ast *first, ...);
#define zz_list(...) _zz_list(__VA_ARGS__, zz_arg_list_end())
struct zz_list zz_append(struct zz_list l, struct zz_ast *a);
struct zz_list zz_prepend(struct zz_list l, struct zz_ast *a);
struct zz_list zz_merge(struct zz_list l, struct zz_list r);

void *zz_arg_list_end(void);

#endif                          // _ZEBU_H
