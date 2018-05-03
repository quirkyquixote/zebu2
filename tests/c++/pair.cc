
#include <cassert>

#include "zebu2.hh"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                ast_get<std::pair<Ast*, Ast*>>(static_cast<const Ast*>(nullptr));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::pair<Ast*, Ast*>>(make_ast(0));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::pair<Ast*, Ast*>>(make_ast(1.0));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::pair<Ast*, Ast*>>(make_ast("foo"s));
                abort();
        } catch(std::exception& ex) {

        }
        try {
                ast_get<std::pair<Ast*, Ast*>>(make_pair(nullptr, nullptr));
        } catch(std::exception& ex) {
                abort();
        }
        return 0;
}
