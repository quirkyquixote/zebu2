
#ifndef _ZEBU_H
#define _ZEBU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc.h>

struct zz_ast;

struct zz_type {
        int (* serialize)(struct zz_ast *, FILE *);
};

const struct zz_type *zz_pair_type(void);
const struct zz_type *zz_int_type(void);
const struct zz_type *zz_ptr_type(void);
const struct zz_type *zz_str_type(void);

struct zz_ast {
        const struct zz_type *type;
};

int zz_print(struct zz_ast *n, FILE *f);

struct zz_pair {
        const struct zz_type *type;
        struct zz_ast* head;
        struct zz_ast* tail;
};

struct zz_ast *zz_pair(struct zz_ast *head, struct zz_ast *tail);
int zz_is_pair(struct zz_ast *n);
struct zz_pair *zz_to_pair(struct zz_ast *n);
struct zz_ast* zz_head(struct zz_ast* a);
struct zz_ast* zz_tail(struct zz_ast* a);

#define zz_foreach(_x, _head) \
for (struct zz_ast* _i = _head; _x = zz_head(_i), _i; _i = zz_tail(_i))

struct zz_int {
        const struct zz_type *type;
        int num;
};

struct zz_ast *zz_int(int num);
int zz_is_int(struct zz_ast *n);
struct zz_int *zz_to_int(struct zz_ast *n);

struct zz_ptr {
        const struct zz_type *type;
        void *ptr;
};

struct zz_ast *zz_ptr(void *ptr);
int zz_is_ptr(struct zz_ast *n);
struct zz_ptr *zz_to_ptr(struct zz_ast *n);

struct zz_str {
        const struct zz_type *type;
        char str[];
};

struct zz_ast *zz_str_with_len(const char *str, int len);
struct zz_ast *zz_str(const char *str);
int zz_is_str(struct zz_ast *n);
struct zz_str *zz_to_str(struct zz_ast *n);

#endif  // _ZEBU_H
