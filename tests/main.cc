
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>

#include "readline/readline.h"
#include "readline/history.h"

extern "C" {
#include "zebu2.h"
};

static const char *ZZ_NUMBER = "number";
static const char *ZZ_STRING = "string";
static const char *ZZ_SYMBOL = "symbol";

class Application {
 public:
        Application()
        {
                zz_ast_mgr_init(&ast);
        }
        ~Application()
        {
                zz_ast_mgr_deinit(&ast);
        }

        void operator()(int argc, char* argv[])
        {
                if (argc > 1) {
                        for (int i = 1; i < argc; ++i) {
                                std::string line;
                                std::ifstream is{argv[i]};
                                std::getline(is, line, '\0');
                                evaluate(line);
                        }
                } else if (isatty(fileno(stdin))) {
                        for (;;) {
                                char* line = readline("$ ");
                                if (line) {
                                        if (*line) {
                                                add_history(line);
                                                evaluate(line);
                                        }
                                        free(line);
                                }
                        }
                } else {
                        std::string line;
                        std::getline(std::cin, line, '\0');
                        evaluate(line);
                }
        }
 private:
        struct zz_ast_mgr ast;
        const char *p;

        void evaluate(const std::string& line)
        {
                try {
                        p = line.data();
                        struct zz_ast *r;
                        while (r = parse_atom()) {
                                zz_print(r, stdout);
                                fputc('\n', stdout);
                                zz_ast_mgr_gc(&ast, r);
                        }
                } catch (std::exception &ex) {
                        std::cerr << ex.what() << "\n";
                }
        }

        struct zz_ast *parse_atom()
        {
                while (isspace(*p))
                        ++p;
                switch (*p) {
                 case '\0':
                        return NULL;
                 case '(':
                        ++p;
                        return parse_list();
                 case ')':
                        throw std::runtime_error{"unexpected ')'"};
                }
                const char *tok =
                        strncmp(p, "number:", 7) == 0 ? ZZ_NUMBER :
                        strncmp(p, "string:", 7) == 0 ? ZZ_STRING :
                        strncmp(p, "symbol:", 7) == 0 ? ZZ_SYMBOL :
                        NULL;
                if (tok == NULL)
                        throw std::runtime_error{"unrecognized token type"};
                p = strchr(p, ':');
                if (p == NULL)
                        throw std::runtime_error{"expected ':'"};
                ++p;
                if (*p != '"')
                        throw std::runtime_error{"expected '\"'"};
                const char *begin = p + 1;
                const char *end = strchr(begin, '"');
                p = end + 1;
                return zz_atom_with_len(&ast, tok, begin, end - begin);
        }

        struct zz_ast *parse_list()
        {
                while (isspace(*p))
                        ++p;
                switch (*p) {
                 case '\0':
                        return NULL;
                 case ')':
                        ++p;
                        return NULL;
                }
                struct zz_ast *n = parse_atom();
                return zz_pair(&ast, n, parse_list());
        }
};

int main(int argc, char* argv[])
{
        Application app;
        app(argc, argv);
}
