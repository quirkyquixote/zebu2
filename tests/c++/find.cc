
#include <algorithm>
#include <cassert>

#include "zebu2.hh"

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

                auto l{make_list(v[0], v[1], v[2], v[3])};
                assert(*std::find(begin(l), end(l), v[0]) == v[0]);
                assert(*std::find(begin(l), end(l), v[1]) == v[1]);
                assert(*std::find(begin(l), end(l), v[2]) == v[2]);
                assert(*std::find(begin(l), end(l), v[3]) == v[3]);
        } catch(std::exception& ex) {
                abort();
        }
}

