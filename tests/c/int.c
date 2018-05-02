
#include <assert.h>
#include <limits.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_int *a;

        GC_init();

        a = (void *)zz_int(0);
        assert(a->type == zz_int_type());
        assert(a->num == 0);

        a = (void *)zz_int(INT_MAX);
        assert(a->type == zz_int_type());
        assert(a->num == INT_MAX);

        a = (void *)zz_int(-1);
        assert(a->type == zz_int_type());
        assert(a->num == -1);
}
