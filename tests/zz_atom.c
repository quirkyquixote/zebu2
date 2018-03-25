
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_atom *a;

        GC_init();

        a = (void *)zz_atom("");
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "") == 0);

        a = (void *)zz_atom("this is a test");
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "this is a test") == 0);

        a = (void *)zz_atom("");
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "") == 0);

        a = (void *)zz_atom("this is a test");
        assert(a->type == zz_atom_type());
        assert(strcmp(a->str, "this is a test") == 0);
}
