
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{

        GC_init();

        {
                struct zz_list l = zz_list(NULL);
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast(zz_str, i)->str);
                assert(strcmp(buf, "") == 0);
        }
        {
                struct zz_list l = zz_list(NULL);
                l = zz_prepend(l, zz_str("foo"));
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast(zz_str, i)->str);
                assert(strcmp(buf, "foo") == 0);
        }
        {
                struct zz_list l = zz_list(NULL);
                l = zz_prepend(l, zz_str("test"));
                l = zz_prepend(l, zz_str("a"));
                l = zz_prepend(l, zz_str("is"));
                l = zz_prepend(l, zz_str("this"));
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast(zz_str, i)->str);
                assert(strcmp(buf, "thisisatest") == 0);
        }
}

