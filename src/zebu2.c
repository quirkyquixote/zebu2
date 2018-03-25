
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
int serialize_int(struct zz_ast *a, FILE *f)
{
        struct zz_int *x = (void *)a;
        return fprintf(f, "%d", x->num);
}
int serialize_ptr(struct zz_ast *a, FILE *f)
{
        struct zz_ptr *x = (void *)a;
        return fprintf(f, "%p", x->ptr);
}
int serialize_str(struct zz_ast *a, FILE *f)
{
        struct zz_str *x = (void *)a;
        return fprintf(f, "%s", x->str);
}

const struct zz_type *zz_pair_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_pair
        };
        return &type;
}
const struct zz_type *zz_int_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_int
        };
        return &type;
}
const struct zz_type *zz_ptr_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_ptr
        };
        return &type;
}
const struct zz_type *zz_str_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_str
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

struct zz_ast *zz_int(int num)
{
        struct zz_int *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_int_type();
        a->num = num;
        return (void *)a;
}
int zz_is_int(struct zz_ast* a)
{
        return a != NULL && a->type == zz_int_type();
}
struct zz_int *zz_to_int(struct zz_ast *a)
{
        return zz_is_int(a) ? (void *)a : NULL;
}

struct zz_ast *zz_ptr(void *ptr)
{
        struct zz_ptr *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_ptr_type();
        a->ptr = ptr;
        return (void *)a;
}
int zz_is_ptr(struct zz_ast* a)
{
        return a != NULL && a->type == zz_ptr_type();
}
struct zz_ptr *zz_to_ptr(struct zz_ast *a)
{
        return zz_is_ptr(a) ? (void *)a : NULL;
}

struct zz_ast *zz_str_with_len(const char *str, int len)
{
        struct zz_str *n = GC_malloc_atomic(sizeof(*n) + len + 1);
        n->type = zz_str_type();
        memcpy(n->str, str, len);
        n->str[len] = 0;
        return (void *)n;
}
struct zz_ast *zz_str(const char *str)
{
        return zz_str_with_len(str, strlen(str));
}
int zz_is_str(struct zz_ast *n)
{
        return n != NULL && n->type == zz_str_type();
}
struct zz_str *zz_to_str(struct zz_ast *n)
{
        return zz_is_str(n) ? (void *)n : NULL;
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
