
#include <cassert>
#include <sstream>

#include "zebu2.hh"

using namespace zz;
using namespace std::string_literals;

int main(int argc, char *argv[])
{
        try {
                std::stringstream buf;
                buf << static_cast<Ast*>(nullptr);
                assert(buf.str() == "()");
        } catch(std::exception& ex) {
                abort();
        }
        try {
                std::stringstream buf;
                buf << make_ast(1);
                assert(buf.str() == "1");
        } catch(std::exception& ex) {
                abort();
        }
        try {
                std::stringstream buf;
                buf << make_ast(1.1);
                assert(buf.str() == "1.1");
        } catch(std::exception& ex) {
                abort();
        }
        try {
                std::stringstream buf;
                buf << make_ast("foo"s);
                assert(buf.str() == "foo");
        } catch(std::exception& ex) {
                abort();
        }
        try {
                std::stringstream buf;
                buf << make_list(nullptr, nullptr);
                assert(buf.str() == "(() ())");
        } catch(std::exception& ex) {
                abort();
        }
        try {
                std::stringstream buf;
                buf << make_list(make_ast(0), make_ast(1), make_ast(2));
                assert(buf.str() == "(0 1 2)");
        } catch(std::exception& ex) {
                abort();
        }
}

