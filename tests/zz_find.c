
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        {
                struct zz_ast *l = NULL;
                struct zz_ast *x = NULL;
                struct zz_ast *r = zz_find(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = NULL;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_find(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = zz_list(zz_str("bar")).first;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_find(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = zz_list(zz_str("foo")).first;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_find(l, x);
                assert(r != NULL);
                assert(zz_cmp(zz_head(r), x) == 0);
        }
        {
                struct zz_ast *l = zz_list(NULL, zz_int(0), zz_str("foo")).first;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_find(l, x);
                assert(r != NULL);
                assert(zz_cmp(zz_head(r), x) == 0);
        }
}
