
#include <algorithm>
#include <cassert>
#include <sstream>
#include <iostream>

#include "zebu2.hh"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        Ast* v[] = {
                nullptr,
                make_ast(0),
                make_ast(1.1),
                make_ast("foo"s)
        };

        try {
                auto l{make_list(v[0], v[1], v[2], v[3])};
                auto l2{make_list(v[1], v[2], v[3])};
                l = remove(l, v[0]);
                assert(std::equal(begin(l2), end(l2), begin(l)));
        } catch(std::exception& ex) {
                abort();
        }
}

