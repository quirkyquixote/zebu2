
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

        assert(zz_cmp(i0, i0) == 0);
        assert(zz_cmp(i0, i1) != 0);
        assert(zz_cmp(i0, pnull) != 0);
        assert(zz_cmp(i0, pself) != 0);
        assert(zz_cmp(i0, sfoo) != 0);
        assert(zz_cmp(i0, sbar) != 0);
        assert(zz_cmp(i0, l0) != 0);
        assert(zz_cmp(i0, li) != 0);
        assert(zz_cmp(i0, lp) != 0);
        assert(zz_cmp(i0, ls) != 0);

        assert(zz_cmp(i1, i0) != 0);
        assert(zz_cmp(i1, i1) == 0);
        assert(zz_cmp(i1, pnull) != 0);
        assert(zz_cmp(i1, pself) != 0);
        assert(zz_cmp(i1, sfoo) != 0);
        assert(zz_cmp(i1, sbar) != 0);
        assert(zz_cmp(i1, l0) != 0);
        assert(zz_cmp(i1, li) != 0);
        assert(zz_cmp(i1, lp) != 0);
        assert(zz_cmp(i1, ls) != 0);

        assert(zz_cmp(pnull, i0) != 0);
        assert(zz_cmp(pnull, i1) != 0);
        assert(zz_cmp(pnull, pnull) == 0);
        assert(zz_cmp(pnull, pself) != 0);
        assert(zz_cmp(pnull, sfoo) != 0);
        assert(zz_cmp(pnull, sbar) != 0);
        assert(zz_cmp(pnull, l0) != 0);
        assert(zz_cmp(pnull, li) != 0);
        assert(zz_cmp(pnull, lp) != 0);
        assert(zz_cmp(pnull, ls) != 0);

        assert(zz_cmp(pself, i0) != 0);
        assert(zz_cmp(pself, i1) != 0);
        assert(zz_cmp(pself, pnull) != 0);
        assert(zz_cmp(pself, pself) == 0);
        assert(zz_cmp(pself, sfoo) != 0);
        assert(zz_cmp(pself, sbar) != 0);
        assert(zz_cmp(pself, l0) != 0);
        assert(zz_cmp(pself, li) != 0);
        assert(zz_cmp(pself, lp) != 0);
        assert(zz_cmp(pself, ls) != 0);

        assert(zz_cmp(sfoo, i0) != 0);
        assert(zz_cmp(sfoo, i1) != 0);
        assert(zz_cmp(sfoo, pnull) != 0);
        assert(zz_cmp(sfoo, pself) != 0);
        assert(zz_cmp(sfoo, sfoo) == 0);
        assert(zz_cmp(sfoo, sbar) != 0);
        assert(zz_cmp(sfoo, l0) != 0);
        assert(zz_cmp(sfoo, li) != 0);
        assert(zz_cmp(sfoo, lp) != 0);
        assert(zz_cmp(sfoo, ls) != 0);

        assert(zz_cmp(sbar, i0) != 0);
        assert(zz_cmp(sbar, i1) != 0);
        assert(zz_cmp(sbar, pnull) != 0);
        assert(zz_cmp(sbar, pself) != 0);
        assert(zz_cmp(sbar, sfoo) != 0);
        assert(zz_cmp(sbar, sbar) == 0);
        assert(zz_cmp(sbar, l0) != 0);
        assert(zz_cmp(sbar, li) != 0);
        assert(zz_cmp(sbar, lp) != 0);
        assert(zz_cmp(sbar, ls) != 0);

        assert(zz_cmp(l0, i0) != 0);
        assert(zz_cmp(l0, i1) != 0);
        assert(zz_cmp(l0, pnull) != 0);
        assert(zz_cmp(l0, pself) != 0);
        assert(zz_cmp(l0, sfoo) != 0);
        assert(zz_cmp(l0, sbar) != 0);
        assert(zz_cmp(l0, l0) == 0);
        assert(zz_cmp(l0, li) != 0);
        assert(zz_cmp(l0, lp) != 0);
        assert(zz_cmp(l0, ls) != 0);

        assert(zz_cmp(li, i0) != 0);
        assert(zz_cmp(li, i1) != 0);
        assert(zz_cmp(li, pnull) != 0);
        assert(zz_cmp(li, pself) != 0);
        assert(zz_cmp(li, sfoo) != 0);
        assert(zz_cmp(li, sbar) != 0);
        assert(zz_cmp(li, l0) != 0);
        assert(zz_cmp(li, li) == 0);
        assert(zz_cmp(li, lp) != 0);
        assert(zz_cmp(li, ls) != 0);

        assert(zz_cmp(lp, i0) != 0);
        assert(zz_cmp(lp, i1) != 0);
        assert(zz_cmp(lp, pnull) != 0);
        assert(zz_cmp(lp, pself) != 0);
        assert(zz_cmp(lp, sfoo) != 0);
        assert(zz_cmp(lp, sbar) != 0);
        assert(zz_cmp(lp, l0) != 0);
        assert(zz_cmp(lp, li) != 0);
        assert(zz_cmp(lp, lp) == 0);
        assert(zz_cmp(lp, ls) != 0);

        assert(zz_cmp(ls, i0) != 0);
        assert(zz_cmp(ls, i1) != 0);
        assert(zz_cmp(ls, pnull) != 0);
        assert(zz_cmp(ls, pself) != 0);
        assert(zz_cmp(ls, sfoo) != 0);
        assert(zz_cmp(ls, sbar) != 0);
        assert(zz_cmp(ls, l0) != 0);
        assert(zz_cmp(ls, li) != 0);
        assert(zz_cmp(ls, lp) != 0);
        assert(zz_cmp(ls, ls) == 0);
}



