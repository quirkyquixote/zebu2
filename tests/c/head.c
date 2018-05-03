
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_ast *a;

        GC_init();

        a = NULL;
        assert(zz_head(a) == NULL);

        a = zz_str("");
        assert(zz_head(a) == a);

        a = zz_str("this is a test");
        assert(zz_head(a) == a);

        a = zz_pair(NULL, NULL);
        assert(zz_head(a) == NULL);

        a = zz_pair(NULL, zz_pair(NULL, NULL));
        assert(zz_head(a) == NULL);

        a = zz_pair(zz_pair(NULL, NULL), NULL);
        assert(zz_head(a) != NULL);

        a = zz_pair(zz_pair(NULL, NULL), zz_pair(NULL, NULL));
        assert(zz_head(a) != NULL);
}
