
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ptr *a;

        a = zz_to_ptr(zz_int(0));
        assert(a == NULL);

        a = zz_to_ptr(zz_ptr(NULL));
        assert(a->type == zz_ptr_type());
        assert(a->ptr == NULL);

        a = zz_to_ptr(zz_str("this is a test"));
        assert(a == NULL);

        a = zz_to_ptr(zz_pair(NULL, NULL));
        assert(a == NULL);
}
