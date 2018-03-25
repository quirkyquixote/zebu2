
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_ast *a;

        GC_init();

        a = NULL;
        assert(zz_tail(a) == NULL);

        a = zz_atom("");
        assert(zz_tail(a) == NULL);

        a = zz_atom("this is a test");
        assert(zz_tail(a) == NULL);

        a = zz_pair(NULL, NULL);
        assert(zz_tail(a) == NULL);

        a = zz_pair(NULL, zz_pair(NULL, NULL));
        assert(zz_tail(a) != NULL);

        a = zz_pair(zz_pair(NULL, NULL), NULL);
        assert(zz_tail(a) == NULL);

        a = zz_pair(zz_pair(NULL, NULL), zz_pair(NULL, NULL));
        assert(zz_tail(a) != NULL);
}
