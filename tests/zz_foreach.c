
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
                strcat(buf, zz_cast(zz_str, i)->str);
        assert(strcmp(buf, "") == 0);

        a = zz_str("this is a test");
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_cast(zz_str, i)->str);
        assert(strcmp(buf, "this is a test") == 0);

        a = zz_pair(zz_str("this "),
                        zz_pair(zz_str("is "),
                                zz_pair(zz_str("a "),
                                        zz_pair(zz_str("test"),
                                                NULL))));
        strcpy(buf, "");
        zz_foreach(i, a)
                strcat(buf, zz_cast(zz_str, i)->str);
        assert(strcmp(buf, "this is a test") == 0);
}
