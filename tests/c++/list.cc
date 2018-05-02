
#include <cassert>

#include "zebu2.hh"

using namespace zz;

int main(int argc, char *argv[])
{
        try {
                Ast a0;
                Ast a1{0};
                Ast a2{1.0};
                Ast a3{"foo"};
                Ast l{a0, Ast{a1, Ast{a2, Ast{a3, Ast{}}}}};
                assert(l[0] == a0);
                assert(l[1] == a1);
                assert(l[2] == a2);
                assert(l[3] == a3);
        } catch(std::exception& ex) {
                abort();
        }
}
