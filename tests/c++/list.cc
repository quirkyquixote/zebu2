
#include <cassert>

#include "zebu2.hh"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                auto it{make_list(nullptr, make_ast(0), make_ast(1.0), make_ast("foo"s))};
                assert(head(it) == nullptr);
                it = tail(it);
                assert(ast_get<int>(head(it)) == 0);
                it = tail(it);
                assert(ast_get<double>(head(it)) == 1.0);
                it = tail(it);
                assert(ast_get<std::string>(head(it)) == "foo"s);
        } catch(std::exception& ex) {
                abort();
        }
}
