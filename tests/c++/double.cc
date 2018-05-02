
#include <cassert>

#include "zebu2.hh"

using namespace zz;

int main(int argc, char *argv[])
{
        try {
                Ast a{0};
                a.get<double>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a;
                a.get<double>();
                abort();
        } catch(std::exception& ex) {

        }
        try {
                Ast a{1.0};
                assert(a.get<double>() == 1.0);
        } catch(std::exception& ex) {

        }
        try {
                Ast a{"foo"};
                a.get<double>();
                abort();
        } catch(std::exception& ex) {

        }
}
