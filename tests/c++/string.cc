
#include <cassert>

#include "zebu2_cpp.h"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                ast_get<std::string>(static_cast<const Ast*>(nullptr));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::string>(make_ast(0));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::string>(make_ast(1.0));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                assert(ast_get<std::string>(make_ast("foo"s)) == "foo"s);
        } catch(std::exception& ex) {

        }
}
