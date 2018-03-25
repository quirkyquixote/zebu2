
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ast *a;

        a = zz_int(0);
        assert(!zz_is_ptr(a));

        a = zz_ptr(NULL);
        assert(zz_is_ptr(a));

        a = zz_str("foo");
        assert(!zz_is_ptr(a));

        a = zz_pair(NULL, NULL);
        assert(!zz_is_ptr(a));
}
