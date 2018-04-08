
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        GC_init();

        struct zz_ast *i0 = zz_int(0);
        struct zz_ast *i1 = zz_int(1);
        struct zz_ast *pnull = zz_ptr(NULL);
        struct zz_ast *pself = zz_ptr(&pself);
        struct zz_ast *sfoo = zz_str("foo");
        struct zz_ast *sbar = zz_str("bar");
        struct zz_ast *l0 = zz_pair(NULL, NULL);
        struct zz_ast *li = zz_list(i0, i1).first;
        struct zz_ast *lp = zz_list(pnull, pself).first;
        struct zz_ast *ls = zz_list(sfoo, sbar).first;

        {
                struct zz_ast *c = zz_copy(i0);
                assert(c != i0 && zz_cmp(c, i0) == 0);
        }
        {
                struct zz_ast *c = zz_copy(i1);
                assert(c != i1 && zz_cmp(c, i1) == 0);
        }
        {
                struct zz_ast *c = zz_copy(pnull);
                assert(c != pnull && zz_cmp(c, pnull) == 0);
        }
        {
                struct zz_ast *c = zz_copy(pself);
                assert(c != pself && zz_cmp(c, pself) == 0);
        }
        {
                struct zz_ast *c = zz_copy(sfoo);
                assert(c != sfoo && zz_cmp(c, sfoo) == 0);
        }
        {
                struct zz_ast *c = zz_copy(sbar);
                assert(c != sbar && zz_cmp(c, sbar) == 0);
        }
        {
                struct zz_ast *c = zz_copy(l0);
                assert(c != l0 && zz_cmp(c, l0) == 0);
        }
        {
                struct zz_ast *c = zz_copy(li);
                assert(c != li && zz_cmp(c, li) == 0);
        }
        {
                struct zz_ast *c = zz_copy(lp);
                assert(c != lp && zz_cmp(c, lp) == 0);
        }
        {
                struct zz_ast *c = zz_copy(ls);
                assert(c != ls && zz_cmp(c, ls) == 0);
        }
}



