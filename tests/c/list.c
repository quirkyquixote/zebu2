
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{

        GC_init();

        {
                struct zz_list l = zz_list_empty();
                struct zz_ast *r = NULL;
                assert(zz_cmp(l.first, r) == 0);
        }
        {
                struct zz_list l = zz_list(NULL);
                struct zz_ast *r = zz_pair(NULL, NULL);
                assert(zz_cmp(l.first, r) == 0);
        }
        {
                struct zz_list l = zz_list(zz_int(0), zz_int(1), zz_int(2));
                struct zz_ast *r = zz_pair(zz_int(0), zz_pair(zz_int(1), zz_pair(zz_int(2), NULL)));
                assert(zz_cmp(l.first, r) == 0);
        }
}


