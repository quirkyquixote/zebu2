
#include "zebu2.h"

#include <stdarg.h>

struct zz_ast *copy_pair(struct zz_ast *a)
{
        struct zz_pair *x = (void *)a;
        return zz_pair(zz_copy(x->head), zz_copy(x->tail));
}
struct zz_ast *copy_int(struct zz_ast *a)
{
        struct zz_int *x = (void *)a;
        return zz_int(x->num);
}
struct zz_ast *copy_ptr(struct zz_ast *a)
{
        struct zz_ptr *x = (void *)a;
        return zz_ptr(x->ptr);
}
struct zz_ast *copy_str(struct zz_ast *a)
{
        struct zz_str *x = (void *)a;
        return zz_str(x->str);
}

int cmp_pair(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_pair *x = (void *)a;
        struct zz_pair *y = (void *)b;
        int ret;
        if ((ret = zz_cmp(x->head, y->head)) != 0)
                return ret;
        if ((ret = zz_cmp(x->tail, y->tail)) != 0)
                return ret;
        return 0;
}
int cmp_int(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_int *x = (void *)a;
        struct zz_int *y = (void *)b;
        return x->num - y->num;
}
int cmp_ptr(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_ptr *x = (void *)a;
        struct zz_ptr *y = (void *)b;
        return x->ptr - y->ptr;
}
int cmp_str(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_str *x = (void *)a;
        struct zz_str *y = (void *)b;
        return strcmp(x->str, y->str);
}

int serialize_pair(struct zz_ast *a, FILE * f)
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

int serialize_int(struct zz_ast *a, FILE * f)
{
        struct zz_int *x = (void *)a;
        return fprintf(f, "%d", x->num);
}

int serialize_ptr(struct zz_ast *a, FILE * f)
{
        struct zz_ptr *x = (void *)a;
        return fprintf(f, "%p", x->ptr);
}

int serialize_str(struct zz_ast *a, FILE * f)
{
        struct zz_str *x = (void *)a;
        return fprintf(f, "%s", x->str);
}

const struct zz_type *zz_pair_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_pair,
                .copy = copy_pair,
                .cmp = cmp_pair,
        };
        return &type;
}

const struct zz_type *zz_int_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_int,
                .copy = copy_int,
                .cmp = cmp_int,
        };
        return &type;
}

const struct zz_type *zz_ptr_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_ptr,
                .copy = copy_ptr,
                .cmp = cmp_ptr,
        };
        return &type;
}

const struct zz_type *zz_str_type(void)
{
        static struct zz_type type = {
                .serialize = serialize_str,
                .copy = copy_str,
                .cmp = cmp_str,
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

struct zz_ast *zz_head(struct zz_ast *a)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        return p == NULL ? a : p->head;
}

struct zz_ast *zz_tail(struct zz_ast *a)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        return p == NULL ? NULL : p->tail;
}

struct zz_ast *zz_int(int num)
{
        struct zz_int *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_int_type();
        a->num = num;
        return (void *)a;
}

struct zz_ast *zz_ptr(void *ptr)
{
        struct zz_ptr *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_ptr_type();
        a->ptr = ptr;
        return (void *)a;
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

int zz_print(struct zz_ast *n, FILE * f)
{
        int ret = 0;
        if (n == NULL) {
                ret += fprintf(f, "()");
        } else if (zz_cast(zz_pair, n)) {
                fputc('(', f);
                ret += n->type->serialize(n, f);
                fputc(')', f);
        } else {
                ret += n->type->serialize(n, f);
        }
        return ret;
}

struct zz_ast *zz_copy(struct zz_ast *a)
{
        return a->type->copy(a);
}
int zz_cmp(struct zz_ast *a, struct zz_ast *b)
{
        int ret = a->type - b->type;
        if (ret != 0)
                return ret;
        return a->type->cmp(a, b);
}

struct zz_list _zz_list(struct zz_ast *first, ...)
{
        struct zz_list ret = (struct zz_list){NULL, NULL};
        if (first != NULL) {
                ret = zz_append(ret, first);
                va_list ap;
                va_start(ap, first);
                struct zz_ast *a;
                while ((a = va_arg(ap, void *)) != NULL)
                        ret = zz_append(ret, a);
                va_end(ap);
        }
        return ret;
}

struct zz_list zz_append(struct zz_list l, struct zz_ast *a)
{
        struct zz_ast *last = zz_pair(a, NULL);
        if (l.first)
                l.last = zz_cast(zz_pair, l.last)->tail = last;
        else
                l.first = l.last = last;
        return l;
}

int _zz_unpack(struct zz_ast *list, ...)
{
        int ret = -1;
        va_list ap;
        va_start(ap, list);
        struct zz_ast *x, *y;
        zz_foreach(x, list) {
                if ((y = va_arg(ap, void *)) == NULL)
                        goto cleanup;
                *(struct zz_ast **)y = x;
        }
        if ((y = va_arg(ap, void *)) != NULL)
                goto cleanup;
        ret = 0;
cleanup:
        va_end(ap);
        return ret;
}

