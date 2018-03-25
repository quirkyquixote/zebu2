
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        static const char *tok_b = "b";
        struct zz_atom *a;

        GC_init();

        a = (void *)zz_atom(tok_a, "");
        assert(a->type == tok_a);
        assert(strcmp(a->str, "") == 0);

        a = (void *)zz_atom(tok_a, "this is a test");
        assert(a->type == tok_a);
        assert(strcmp(a->str, "this is a test") == 0);

        a = (void *)zz_atom(tok_b, "");
        assert(a->type == tok_b);
        assert(strcmp(a->str, "") == 0);

        a = (void *)zz_atom(tok_b, "this is a test");
        assert(a->type == tok_b);
        assert(strcmp(a->str, "this is a test") == 0);
}
