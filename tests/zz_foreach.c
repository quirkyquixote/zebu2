
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        static const char *tok_a = "a";
        static const char *tok_b = "b";
        struct zz_ast *a, *i;
        char buf[1024];

        GC_init();

        a = NULL;
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "") == 0);

        a = zz_atom(tok_a, "this is a test");
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "this is a test") == 0);

        a = zz_pair(zz_atom(tok_a, "this "),
                        zz_pair(zz_atom(tok_a, "is "),
                                zz_pair(zz_atom(tok_a, "a "),
                                        zz_pair(zz_atom(tok_a, "test"),
                                                NULL))));
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "this is a test") == 0);
}
