
#ifndef ZEBU_TEST_H_
#define ZEBU_TEST_H_

#include "zebu2.h"

struct array {
        int len;
        int cap;
        int siz;
        char *buf;
};

#define array(siz) (struct array){0, 0, siz, NULL}
void *array_index(struct array *a, int i);
void array_resize(struct array *a, int len);
void array_clear(struct array *a);
void array_replace(struct array *a, int i, void *p);
void array_insert(struct array *a, int i, void *p);
void array_erase(struct array *a, int i);
void *array_front(struct array *a);
void *array_back(struct array *a);
void array_push_back(struct array *a, void *p);
void array_push_front(struct array *a, void *p);
void array_pop_back(struct array *a);
void array_pop_front(struct array *a);

struct zz_ast *prune(struct zz_ast *a, int first);

int yylex();
int yyparse();
void yyerror();

#endif                          // ZEBU_TEST_H_
