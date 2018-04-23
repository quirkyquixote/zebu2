
#include <assert.h>

#include "zebu2.h"

int main(int argc, char* argv[])
{
        {
                struct zz_ast *a = zz_list(NULL).first;
                zz_set_index(a, 0, zz_int(0));
                assert(zz_cmp(a, zz_list(zz_int(0)).first) == 0);
        }
        {
                struct zz_ast *a = zz_list(NULL).first;
                zz_set_index(a, 0, zz_str("foo"));
                assert(zz_cmp(a, zz_list(zz_str("foo")).first) == 0);
        }
        {
                struct zz_ast *a = zz_list(NULL).first;
                zz_set_index(a, 0, zz_pair(NULL, NULL));
                assert(zz_cmp(a, zz_list(zz_pair(NULL, NULL)).first) == 0);
        }
        {
                struct zz_ast *a = zz_list(NULL, NULL, NULL).first;
                zz_set_index(a, 0, zz_int(0));
                zz_set_index(a, 1, zz_str("foo"));
                zz_set_index(a, 2, zz_pair(NULL, NULL));
                assert(zz_cmp(a, zz_list(zz_int(0), zz_str("foo"), zz_pair(NULL, NULL)).first) == 0);
        }
}
