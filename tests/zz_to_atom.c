
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        GC_init();

        struct zz_atom *a;

        a = zz_to_atom(zz_atom(tok_a, ""));
        assert(a->type == tok_a);
        assert(strcmp(a->str, "") == 0);

        a = zz_to_atom(zz_atom(tok_a, "this is a test"));
        assert(a->type == tok_a);
        assert(strcmp(a->str, "this is a test") == 0);

        a = zz_to_atom(zz_pair(NULL, NULL));
        assert(a == NULL);
}
