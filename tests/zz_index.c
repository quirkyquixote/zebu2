
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ast *i0 = zz_int(0);
        struct zz_ast *i1 = zz_int(1);
        struct zz_ast *i2 = zz_int(2);
        struct zz_ast *i3 = zz_int(3);
        struct zz_ast *i4 = zz_int(4);
        struct zz_ast *i5 = zz_int(5);
        struct zz_ast *l = zz_list(i0, i1, i2, i3, i4, i5).first;

        assert(zz_index(i0, 0) == i0);
        assert(zz_index(i0, 1) == NULL);
        assert(zz_index(l, 0) == i0);
        assert(zz_index(l, 1) == i1);
        assert(zz_index(l, 2) == i2);
        assert(zz_index(l, 3) == i3);
        assert(zz_index(l, 4) == i4);
        assert(zz_index(l, 5) == i5);
        assert(zz_index(l, 6) == NULL);
        assert(zz_index(l, 7) == NULL);
        assert(zz_index(l, 8) == NULL);
}




