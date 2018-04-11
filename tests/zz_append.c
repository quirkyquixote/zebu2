
#include <assert.h>

#include "zebu2.h"

int main(int argc, char *argv[])
{

        GC_init();

        {
                struct zz_list l = zz_list_empty();
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast_or_die(zz_str, i)->str);
                assert(strcmp(buf, "") == 0);
        }
        {
                struct zz_list l = zz_list_empty();
                l = zz_append(l, zz_str("foo"));
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast_or_die(zz_str, i)->str);
                assert(strcmp(buf, "foo") == 0);
        }
        {
                struct zz_list l = zz_list_empty();
                l = zz_append(l, zz_str("this"));
                l = zz_append(l, zz_str("is"));
                l = zz_append(l, zz_str("a"));
                l = zz_append(l, zz_str("test"));
                char buf[1024];
                struct zz_ast *i;
                strcpy(buf, "");
                zz_foreach(i, l.first)
                        strcat(buf, zz_cast_or_die(zz_str, i)->str);
                assert(strcmp(buf, "thisisatest") == 0);
        }
}

