
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_pair *a;

        a = zz_to_pair(zz_atom(""));
        assert(a == NULL);

        a = zz_to_pair(zz_atom("this is a test"));
        assert(a == NULL);

        a = zz_to_pair(zz_pair(NULL, NULL));
        assert(a != NULL);
        assert(a->head == NULL);
        assert(a->tail == NULL);
}
