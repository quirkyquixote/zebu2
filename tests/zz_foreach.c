
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{
        struct zz_ast *a, *i;
        char buf[1024];

        GC_init();

        a = NULL;
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "") == 0);

        a = zz_atom("this is a test");
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "this is a test") == 0);

        a = zz_pair(zz_atom("this "),
                        zz_pair(zz_atom("is "),
                                zz_pair(zz_atom("a "),
                                        zz_pair(zz_atom("test"),
                                                NULL))));
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_to_atom(i)->str);
        assert(strcmp(buf, "this is a test") == 0);
}
