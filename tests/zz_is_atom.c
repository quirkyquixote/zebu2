
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ast *a;

        a = zz_atom("");
        assert(zz_is_atom(a));

        a = zz_atom("this is a test");
        assert(zz_is_atom(a));

        a = zz_pair(NULL, NULL);
        assert(!zz_is_atom(a));
}
