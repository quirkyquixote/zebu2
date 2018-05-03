

#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{

        GC_init();

        {
                struct zz_ast *l = NULL;
                struct zz_ast *a1, *a2, *a3;
                assert(zz_unpack(l) == 0);
                assert(zz_unpack(l, &a1) == 0);
                assert(zz_unpack(l, &a1, &a2) == 0);
                assert(zz_unpack(l, &a1, &a2, &a3) == 0);
                assert(zz_unpack(l, NULL) == 0);
                assert(zz_unpack(l, NULL, NULL) == 0);
                assert(zz_unpack(l, NULL, NULL, NULL) == 0);
        }
        {
                struct zz_ast *l = zz_list(zz_int(0)).first;
                struct zz_ast *a1, *a2, *a3;
                assert(zz_unpack(l) == 0);
                assert(zz_unpack(l, &a1) == 1);
                assert(zz_unpack(l, &a1, &a2) == 1);
                assert(zz_unpack(l, &a1, &a2, &a3) == 1);
                assert(zz_unpack(l, NULL) == 1);
                assert(zz_unpack(l, NULL, NULL) == 1);
                assert(zz_unpack(l, NULL, NULL, NULL) == 1);
        }
        {
                struct zz_ast *l = zz_list(zz_int(0), zz_int(1), zz_int(2)).first;
                struct zz_ast *a1, *a2, *a3;
                assert(zz_unpack(l) == 0);
                assert(zz_unpack(l, &a1) == 1);
                assert(zz_unpack(l, &a1, &a2) == 2);
                assert(zz_unpack(l, &a1, &a2, &a3) == 3);
                assert(zz_unpack(l, NULL) == 1);
                assert(zz_unpack(l, NULL, NULL) == 2);
                assert(zz_unpack(l, NULL, NULL, NULL) == 3);
        }
}

