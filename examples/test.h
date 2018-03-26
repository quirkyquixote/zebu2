
#ifndef ZEBU_TEST_H_
#define ZEBU_TEST_H_

#include "zebu2.h"

struct zz_ast *prune(struct zz_ast *a, int first);

int yylex();
int yyparse(const char **ptr);
void yyerror(const char **ptr, const char *msg);

#endif                          // ZEBU_TEST_H_
