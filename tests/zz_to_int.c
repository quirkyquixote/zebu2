
#include <limits.h>
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_int *a;

        a = zz_to_int(zz_int(0));
        assert(a->type == zz_int_type());
        assert(a->num == 0);

        a = zz_to_int(zz_int(INT_MAX));
        assert(a->type == zz_int_type());
        assert(a->num == INT_MAX);

        a = zz_to_int(zz_ptr(NULL));
        assert(a == NULL);

        a = zz_to_int(zz_str("this is a test"));
        assert(a == NULL);

        a = zz_to_int(zz_pair(NULL, NULL));
        assert(a == NULL);
}
