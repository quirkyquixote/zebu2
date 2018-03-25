
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ast *a;

        a = zz_str("");
        assert(!zz_is_pair(a));

        a = zz_str("this is a test");
        assert(!zz_is_pair(a));

        a = zz_pair(NULL, NULL);
        assert(zz_is_pair(a));

        a = zz_pair(zz_pair(NULL, NULL), NULL);
        assert(zz_is_pair(a));

        a = zz_pair(NULL, zz_pair(NULL, NULL));
        assert(zz_is_pair(a));

        a = zz_pair(zz_pair(NULL, NULL), zz_pair(NULL, NULL));
        assert(zz_is_pair(a));
}
