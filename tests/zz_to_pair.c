
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        GC_init();

        struct zz_pair *a;

        a = zz_to_pair(zz_atom(tok_a, ""));
        assert(a == NULL);

        a = zz_to_pair(zz_atom(tok_a, "this is a test"));
        assert(a == NULL);

        a = zz_to_pair(zz_pair(NULL, NULL));
        assert(a != NULL);
        assert(a->head == NULL);
        assert(a->tail == NULL);
}
