
#include <cassert>

#include "zebu2_cpp.h"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                Ast* v[] = {
                        nullptr,
                        make_ast(0),
                        make_ast(1.0),
                        make_ast("foo"s)
                };

                auto it{make_list(v[0], v[1], v[2], v[3])};

                int i = 0;
                for (auto x : it)
                        assert(x == v[i++]);
        } catch(std::exception& ex) {
                abort();
        }
}

