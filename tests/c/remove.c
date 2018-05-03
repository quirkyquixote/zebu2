
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        {
                struct zz_ast *l = NULL;
                struct zz_ast *x = NULL;
                struct zz_ast *r = zz_remove(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = NULL;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_remove(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = zz_list(zz_str("bar")).first;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_remove(l, x);
                assert(r == l);
        }
        {
                struct zz_ast *l = zz_list(zz_str("foo")).first;
                struct zz_ast *x = zz_str("foo");
                struct zz_ast *r = zz_remove(l, x);
                assert(r == NULL);
        }
        {
                struct zz_ast *l = zz_list(NULL, zz_int(0), zz_str("foo")).first;
                struct zz_ast *r = zz_remove(l, NULL);
                assert(zz_cmp(r, zz_list(zz_int(0), zz_str("foo")).first) == 0);
        }
        {
                struct zz_ast *l = zz_list(NULL, zz_int(0), zz_str("foo")).first;
                struct zz_ast *r = zz_remove(l, zz_int(0));
                assert(zz_cmp(r, zz_list(NULL, zz_str("foo")).first) == 0);
        }
        {
                struct zz_ast *l = zz_list(NULL, zz_int(0), zz_str("foo")).first;
                struct zz_ast *r = zz_remove(l, zz_str("foo"));
                assert(zz_cmp(r, zz_list(NULL, zz_int(0)).first) == 0);
        }
        {
                struct zz_ast *l = zz_list(NULL, NULL, NULL).first;
                struct zz_ast *r = zz_remove(l, NULL);
                assert(r == NULL);
        }
}
