
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_str *a;

        a = zz_to_str(zz_str(""));
        assert(a->type == zz_str_type());
        assert(strcmp(a->str, "") == 0);

        a = zz_to_str(zz_str("this is a test"));
        assert(a->type == zz_str_type());
        assert(strcmp(a->str, "this is a test") == 0);

        a = zz_to_str(zz_pair(NULL, NULL));
        assert(a == NULL);
}
