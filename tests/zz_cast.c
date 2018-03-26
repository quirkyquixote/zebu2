
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        {
                struct zz_int *a;
                a = zz_cast(zz_int, zz_int(0));
                assert(a != NULL);
                assert(a->type == zz_int_type());
                assert(a->num == 0);

                a = zz_cast(zz_int, zz_ptr(NULL));
                assert(a == NULL);

                a = zz_cast(zz_int, zz_str("this is a test"));
                assert(a == NULL);

                a = zz_cast(zz_int, zz_pair(NULL, NULL));
                assert(a == NULL);
        }
        {
                struct zz_ptr *a;
                a = zz_cast(zz_ptr, zz_int(0));
                assert(a == NULL);

                a = zz_cast(zz_ptr, zz_ptr(NULL));
                assert(a != NULL);
                assert(a->type == zz_ptr_type());
                assert(a->ptr == NULL);

                a = zz_cast(zz_ptr, zz_str("this is a test"));
                assert(a == NULL);

                a = zz_cast(zz_ptr, zz_pair(NULL, NULL));
                assert(a == NULL);
        }
        {
                struct zz_str *a;
                a = zz_cast(zz_str, zz_int(0));
                assert(a == NULL);

                a = zz_cast(zz_str, zz_ptr(NULL));
                assert(a == NULL);

                a = zz_cast(zz_str, zz_str("this is a test"));
                assert(a != NULL);
                assert(a->type == zz_str_type());
                assert(strcmp(a->str, "this is a test") == 0);

                a = zz_cast(zz_str, zz_pair(NULL, NULL));
                assert(a == NULL);
        }
        {
                struct zz_pair *a;
                a = zz_cast(zz_pair, zz_int(0));
                assert(a == NULL);

                a = zz_cast(zz_pair, zz_ptr(NULL));
                assert(a == NULL);

                a = zz_cast(zz_pair, zz_str("this is a test"));
                assert(a == NULL);

                a = zz_cast(zz_pair, zz_pair(NULL, NULL));
                assert(a != NULL);
                assert(a->type == zz_pair_type());
                assert(a->head == NULL);
                assert(a->tail == NULL);
        }
}
