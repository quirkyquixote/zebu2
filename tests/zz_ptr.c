
#include <assert.h>
#include <limits.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_ptr *a;

        GC_init();

        a = (void *)zz_ptr(NULL);
        assert(a->type == zz_ptr_type());
        assert(a->ptr == 0);

        a = (void *)zz_ptr(&a);
        assert(a->type == zz_ptr_type());
        assert(a->ptr == &a);
}
