
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_ast *i0 = zz_int(0);
        struct zz_ast *i1 = zz_int(1);
        struct zz_ast *i2 = zz_int(2);
        struct zz_ast *i3 = zz_int(3);

        GC_init();

        {
                struct zz_list l = zz_list_empty();
                struct zz_list r = zz_list_empty();
                struct zz_list m = zz_list_empty();
                assert(zz_cmp(zz_merge(l, r).first, m.first) == 0);
        }
        {
                struct zz_list l = zz_list_empty();
                struct zz_list r = zz_list(i0, i1, i2, i3);
                struct zz_list m = zz_list(i0, i1, i2, i3);
                assert(zz_cmp(zz_merge(l, r).first, m.first) == 0);
        }
        {
                struct zz_list l = zz_list(i0, i1, i2, i3);
                struct zz_list r = zz_list_empty();
                struct zz_list m = zz_list(i0, i1, i2, i3);
                assert(zz_cmp(zz_merge(l, r).first, m.first) == 0);
        }
        {
                struct zz_list l = zz_list(i0, i1);
                struct zz_list r = zz_list(i2, i3);
                struct zz_list m = zz_list(i0, i1, i2, i3);
                assert(zz_cmp(zz_merge(l, r).first, m.first) == 0);
        }
        return 0;
}
