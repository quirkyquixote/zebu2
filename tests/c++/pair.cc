
#include <cassert>

#include "zebu2.hh"

using namespace zz;

int main(int argc, char *argv[])
{
        try {
                Ast a{0};
                a.get<std::pair<Ast, Ast>>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a;
                a.get<std::pair<Ast, Ast>>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{1.0};
                a.get<std::pair<Ast, Ast>>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{"foo"};
                a.get<std::pair<Ast, Ast>>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{Ast{}, Ast{}};
                a.get<std::pair<Ast, Ast>>();
        } catch(std::exception& ex) {
                abort();
        }
}
