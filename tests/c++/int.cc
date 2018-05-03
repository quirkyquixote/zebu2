
#include <cassert>

#include "zebu2_cpp.h"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                ast_get<int>(static_cast<const Ast*>(nullptr));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                assert(ast_get<int>(make_ast(0)) == 0);
        } catch(std::exception& ex) {

        }
        try {
                ast_get<int>(make_ast(1.0));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<int>(make_ast("foo"s));
                abort();
        } catch(std::exception& ex) {

        }
}
