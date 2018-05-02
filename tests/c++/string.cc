
#include <cassert>

#include "zebu2.hh"

using namespace zz;

int main(int argc, char *argv[])
{
        try {
                Ast a{0};
                a.get<const char *>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a;
                a.get<const char *>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{1.0};
                a.get<const char *>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{"foo"};
                assert(a.get<const char *>() == "foo");
        } catch(std::exception& ex) {

        }
}
