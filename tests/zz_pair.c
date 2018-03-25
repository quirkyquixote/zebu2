
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        static const char *tok_b = "b";
        struct zz_pair *a;

        GC_init();

        a = (void *)zz_pair(NULL, NULL);
        assert(a->type == ZZ_PAIR);
        assert(a->head == NULL);
        assert(a->tail == NULL);

        a = (void *)zz_pair(NULL, zz_pair(NULL, NULL));
        assert(a->type == ZZ_PAIR);
        assert(a->head == NULL);
        assert(a->tail != NULL);
        assert(a->tail->type == ZZ_PAIR);

        a = (void *)zz_pair(zz_pair(NULL, NULL), NULL);
        assert(a->type == ZZ_PAIR);
        assert(a->head != NULL);
        assert(a->head->type == ZZ_PAIR);
        assert(a->tail == NULL);

        a = (void *)zz_pair(zz_pair(NULL, NULL), zz_pair(NULL, NULL));
        assert(a->type == ZZ_PAIR);
        assert(a->head != NULL);
        assert(a->head->type == ZZ_PAIR);
        assert(a->tail != NULL);
        assert(a->tail->type == ZZ_PAIR);
}
