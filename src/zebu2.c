
#include "zebu2.h"

#include <assert.h>
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
                .name = "zz_pair",
                .serialize = serialize_pair,
                .copy = copy_pair,
                .cmp = cmp_pair,
        };
        return &type;
}

const struct zz_type *zz_int_type(void)
{
        static struct zz_type type = {
                .name = "zz_int",
                .serialize = serialize_int,
                .copy = copy_int,
                .cmp = cmp_int,
        };
        return &type;
}

const struct zz_type *zz_ptr_type(void)
{
        static struct zz_type type = {
                .name = "zz_ptr",
                .serialize = serialize_ptr,
                .copy = copy_ptr,
                .cmp = cmp_ptr,
        };
        return &type;
}

const struct zz_type *zz_str_type(void)
{
        static struct zz_type type = {
                .name = "zz_str",
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
        struct zz_pair *p = zz_cast_or_null(zz_pair, a);
        return p == NULL ? a : p->head;
}

struct zz_ast *zz_tail(struct zz_ast *a)
{
        struct zz_pair *p = zz_cast_or_null(zz_pair, a);
        return p == NULL ? NULL : p->tail;
}

struct zz_ast *zz_insert(struct zz_ast *a, struct zz_ast *next)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        return (p->tail = zz_pair(next, p->tail));
}

void zz_replace(struct zz_ast *a, struct zz_ast *head)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        p->head = head;
}

int _zz_unpack(struct zz_ast *list, ...)
{
        int ret = 0;
        va_list ap;
        va_start(ap, list);
        struct zz_ast *x, *y;
        zz_foreach(x, list) {
                if ((y = va_arg(ap, void *)) == zz_arg_list_end())
                        goto cleanup;
                if (y != NULL)
                        *(struct zz_ast **)y = x;
                ++ret;
        }
cleanup:
        va_end(ap);
        return ret;
}

struct zz_ast *zz_index(struct zz_ast *a, int i)
{
        while (i--)
                a = zz_tail(a);
        return zz_head(a);
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
        } else if (zz_typeof(n) == zz_pair_type()) {
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

struct zz_list zz_list_empty(void)
{
        return (struct zz_list){NULL, NULL};
}

struct zz_list _zz_list(struct zz_ast *first, ...)
{
        struct zz_list ret;
        ret.first = ret.last = zz_pair(first, NULL);
        va_list ap;
        va_start(ap, first);
        struct zz_ast *a;
        while ((a = va_arg(ap, void *)) != zz_arg_list_end())
                ret = zz_append(ret, a);
        va_end(ap);
        return ret;
}

struct zz_list zz_append(struct zz_list l, struct zz_ast *a)
{
        if (l.last)
                l.last = zz_insert(l.last, a);
        else
                l.last = zz_pair(a, NULL);
        if (l.first == NULL)
                l.first = l.last;
        return l;
}

struct zz_list zz_prepend(struct zz_list l, struct zz_ast *a)
{
        l.first = zz_pair(a, l.first);
        if (l.last == NULL)
                l.last = l.first;
        return l;
}

struct zz_list zz_merge(struct zz_list l, struct zz_list r)
{
        if (r.first == NULL)
                return l;
        if (l.first == NULL)
                return r;
        zz_cast(zz_pair, l.last)->tail = r.first;
        l.last = r.last;
        return l;
}

void *zz_arg_list_end(void)
{
        static struct {} val;
        return &val;
}

