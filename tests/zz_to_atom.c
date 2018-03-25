
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_atom *a;

        a = zz_to_atom(zz_atom(""));
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "") == 0);

        a = zz_to_atom(zz_atom("this is a test"));
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "this is a test") == 0);

        a = zz_to_atom(zz_pair(NULL, NULL));
        assert(a == NULL);
}
