
#include <assert.h>

#include "zebu2.h"

int is_sorted(struct zz_ast *a, int(* cmp)(struct zz_ast*, struct zz_ast*))
{
        struct zz_ast *x, *y;
        if (zz_unpack(a, &x, &y) != 2)
                return 1;
        if (cmp(x, y) > 0)
                return 0;
        return is_sorted(zz_tail(a), cmp);
}

int main(int argc, char *argv[])
{
        {
                struct zz_ast *a = NULL;
                assert(is_sorted(zz_sort(a, zz_cmp), zz_cmp));
        }
        {
                struct zz_ast *a = zz_list(NULL).first;
                assert(is_sorted(zz_sort(a, zz_cmp), zz_cmp));
        }
        {
                struct zz_ast *a = zz_list(zz_int(0)).first;
                assert(is_sorted(zz_sort(a, zz_cmp), zz_cmp));
        }
        {
                struct zz_ast *a = zz_list(zz_str("foo")).first;
                assert(is_sorted(zz_sort(a, zz_cmp), zz_cmp));
        }
        {
                struct zz_ast *a = zz_list(zz_int(3), zz_int(1), zz_int(0)).first;
                assert(is_sorted(zz_sort(a, zz_cmp), zz_cmp));
        }
}
