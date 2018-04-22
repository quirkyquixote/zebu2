
#include "zebu2.h"

#include <assert.h>
#include <stdarg.h>

struct zz_ast *copy_null(struct zz_ast *a)
{
        return NULL;
}
struct zz_ast *copy_pair(struct zz_ast *a)
{
        struct zz_pair *x = (void *)a;
        return zz_pair(zz_copy(x->head), zz_copy(x->tail));
}
struct zz_ast *copy_int(struct zz_ast *a)
{
        return a;
}
struct zz_ast *copy_double(struct zz_ast *a)
{
        return a;
}
struct zz_ast *copy_ptr(struct zz_ast *a)
{
        return a;
}
struct zz_ast *copy_str(struct zz_ast *a)
{
        return a;
}

int cmp_null(struct zz_ast *a, struct zz_ast *b)
{
        return 0;
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
int cmp_double(struct zz_ast *a, struct zz_ast *b)
{
        struct zz_double *x = (void *)a;
        struct zz_double *y = (void *)b;
        return (x->num < y->num) ? -1 : (x->num > y->num) ? 1 : 0;
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

int serialize_null(struct zz_ast *a, FILE * f)
{
        return fprintf(f, "()");
}
int serialize_pair(struct zz_ast *a, FILE * f)
{
        struct zz_pair *x = (void *)a;
        int ret = 0;
        ret += zz_print(x->head, f);
        if (x->tail) {
                fputc(' ', f);
                ret += zz_typeof(x->tail)->serialize(x->tail, f);
        }
        return ret;
}
int serialize_int(struct zz_ast *a, FILE * f)
{
        struct zz_int *x = (void *)a;
        return fprintf(f, "%d", x->num);
}
int serialize_double(struct zz_ast *a, FILE * f)
{
        struct zz_double *x = (void *)a;
        return fprintf(f, "%g", x->num);
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

const struct zz_type *zz_null_type(void)
{
        static struct zz_type type = {
                .name = "Null",
                .serialize = serialize_null,
                .copy = copy_null,
                .cmp = cmp_null,
        };
        return &type;
}

const struct zz_type *zz_pair_type(void)
{
        static struct zz_type type = {
                .name = "Pair",
                .serialize = serialize_pair,
                .copy = copy_pair,
                .cmp = cmp_pair,
        };
        return &type;
}

const struct zz_type *zz_int_type(void)
{
        static struct zz_type type = {
                .name = "Int",
                .serialize = serialize_int,
                .copy = copy_int,
                .cmp = cmp_int,
        };
        return &type;
}

const struct zz_type *zz_double_type(void)
{
        static struct zz_type type = {
                .name = "Double",
                .serialize = serialize_double,
                .copy = copy_double,
                .cmp = cmp_double,
        };
        return &type;
}

const struct zz_type *zz_ptr_type(void)
{
        static struct zz_type type = {
                .name = "Ptr",
                .serialize = serialize_ptr,
                .copy = copy_ptr,
                .cmp = cmp_ptr,
        };
        return &type;
}

const struct zz_type *zz_str_type(void)
{
        static struct zz_type type = {
                .name = "Str",
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

void zz_set_head(struct zz_ast *a, struct zz_ast *head)
{
        zz_cast(zz_pair, a)->head = head;
}

void zz_set_tail(struct zz_ast *a, struct zz_ast *tail)
{
        zz_cast(zz_pair, a)->tail = tail;
}

struct zz_ast *zz_insert(struct zz_ast *a, struct zz_ast *next)
{
        struct zz_pair *p = zz_cast(zz_pair, a);
        return (p->tail = zz_pair(next, p->tail));
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

struct zz_ast *zz_find(struct zz_ast *l, struct zz_ast *x)
{
        for (; l != NULL; l = zz_tail(l)) {
                if (zz_cmp(zz_head(l), x) == 0)
                        return l;
        }
        return l;
}
struct zz_ast *zz_find_if(struct zz_ast *l,
                int(* func)(struct zz_ast *, void *), void *data)
{
        for (; l != NULL; l = zz_tail(l)) {
                if (func(zz_head(l), data))
                        return l;
        }
        return l;
}

struct zz_ast *zz_remove(struct zz_ast *l, struct zz_ast *x)
{
        for (struct zz_pair **i = (void *)&l; *i != NULL;) {
                if (zz_cmp((*i)->head, x) == 0)
                        *i = (void *)(*i)->tail;
                else
                        i = (void *)&((*i)->tail);

        }
        return l;
}
struct zz_ast *zz_remove_if(struct zz_ast *l,
                int(* func)(struct zz_ast *, void *), void *data)
{
        for (struct zz_pair **i = (void *)&l; *i != NULL;) {
                if (func((*i)->head, data))
                        *i = (void *)(*i)->tail;
                else
                        i = (void *)&((*i)->tail);

        }
        return l;
}

struct zz_ast *merge_sort(struct zz_ast *l, struct zz_ast *r,
                int(* cmp)(struct zz_ast *, struct zz_ast *))
{
        if (l == NULL)
                return r;
        if (r == NULL )
                return l;
        if (cmp(zz_head(l), zz_head(r)) <  0) {
                struct zz_pair *p = zz_cast(zz_pair, l);
                p->tail = merge_sort(p->tail, r, cmp);
                return l;
        } else {
                struct zz_pair *p = zz_cast(zz_pair, r);
                p->tail = merge_sort(l, p->tail, cmp);
                return r;
        }
}
struct zz_ast *zz_sort(struct zz_ast *l,
                int(* cmp)(struct zz_ast *, struct zz_ast *))
{
        if (l == NULL || zz_tail(l) == NULL)
                return l;
        struct zz_ast *m = l, *r = l;
        for (;;) {
                if ((r = zz_tail(r)) == NULL)
                        break;
                if ((r = zz_tail(r)) == NULL)
                        break;
                m = zz_tail(m);
        }
        r = zz_tail(m);
        zz_cast(zz_pair, m)->tail = NULL;
        return merge_sort(zz_sort(l, cmp), zz_sort(r, cmp), cmp);
}

struct zz_ast *zz_int(int num)
{
        struct zz_int *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_int_type();
        a->num = num;
        return (void *)a;
}

struct zz_ast *zz_double(double num)
{
        struct zz_double *a = GC_malloc_atomic(sizeof(*a));
        a->type = zz_double_type();
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
        if (zz_typeof(n) == zz_pair_type()) {
                fputc('(', f);
                ret += zz_typeof(n)->serialize(n, f);
                fputc(')', f);
        } else {
                ret += zz_typeof(n)->serialize(n, f);
        }
        return ret;
}

struct zz_ast *zz_copy(struct zz_ast *a)
{
        return zz_typeof(a)->copy(a);
}
int zz_cmp(struct zz_ast *a, struct zz_ast *b)
{
        int ret = zz_typeof(a) - zz_typeof(b);
        if (ret != 0)
                return ret;
        return zz_typeof(a)->cmp(a, b);
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

