
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        GC_init();

        struct zz_ast *a;

        a = zz_atom(tok_a, "");
        assert(zz_is_atom(a));

        a = zz_atom(tok_a, "this is a test");
        assert(zz_is_atom(a));

        a = zz_pair(NULL, NULL);
        assert(!zz_is_atom(a));
}
