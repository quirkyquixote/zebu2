
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_pair *a;

        GC_init();

        a = (void *)zz_pair(NULL, NULL);
        assert(a->type == zz_pair_type());
        assert(a->head == NULL);
        assert(a->tail == NULL);

        a = (void *)zz_pair(NULL, zz_pair(NULL, NULL));
        assert(a->type == zz_pair_type());
        assert(a->head == NULL);
        assert(a->tail != NULL);
        assert(a->tail->type == zz_pair_type());

        a = (void *)zz_pair(zz_pair(NULL, NULL), NULL);
        assert(a->type == zz_pair_type());
        assert(a->head != NULL);
        assert(a->head->type == zz_pair_type());
        assert(a->tail == NULL);

        a = (void *)zz_pair(zz_pair(NULL, NULL), zz_pair(NULL, NULL));
        assert(a->type == zz_pair_type());
        assert(a->head != NULL);
        assert(a->head->type == zz_pair_type());
        assert(a->tail != NULL);
        assert(a->tail->type == zz_pair_type());
}
