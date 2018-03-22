Usage
=====

.. highlight:: c

A basic example in which an AST is built from a grammar similar to the one used
in the reverse polish notation calculator example in the GNU Bison
documentation

Declarations
------------

::

    %{
    /*
     * These are the tokens that define node types in the AST
     */
    extern const char *TOK_INPUT;
    extern const char *TOK_NUM;
    extern const char *TOK_ADD;
    extern const char *TOK_SUB;
    extern const char *TOK_MUL;
    extern const char *TOK_DIV;
    extern const char *TOK_EXP;
    extern const char *TOK_NEG;
    %}

    /*
     * The ast is a factory for nodes, and must be passed from outside.
     * Depending on your mode of operation, a return location for the root
     * node may be required, too.
     */
    %parse-param { struct zz_ast_mgr *ast }

    /*
     * At least one possible data type will be struct zz_ast; all of them may
     * be, if the lexer has access to the zz_ast_mgr object and can construct
     * nodes by itself.
     */
    %union {
        int number;
        struct zz_ast *node;
    }

    /*
     * Token and rule declarations
     */
    %token '\n'
    %token '+'
    %token '-'
    %token '*'
    %token '/'
    %token 'n'
    %token <number> NUM
    %type <ast> input
    %type <ast> line
    %type <ast> exp


Grammar Rules
-------------

::

    /*
     * To build a list of any size, the empty rule generates a NULL terminator
     * and the rest of the list is generated recursively
     */
    input
        : {
            $$ = NULL;
            }
        | input line {
            $$ = zz_pair(ast, $1, $2);
            }
        ;

    /*
     * Pass the ast upwards
     */
    line
        : exp '\n' {
            return $$;
            }
        ;

    exp
        : NUM {
            $$ = zz_atom(ast, TOK_NUM, $1);
            }
        | exp exp '+'     { 
            $$ = zz_pair(ast, zz_atom(ast, TOK_ADD, ""),
                 zz_pair(ast, $1,
                 zz_pair(ast, $2,
                 NULL)));
            }
        | exp exp '-'     { 
            $$ = zz_pair(ast, zz_atom(ast, TOK_SUB, ""),
                 zz_pair(ast, $1,
                 zz_pair(ast, $2,
                 NULL)));
            }
        | exp exp '*'     { 
            $$ = zz_pair(ast, zz_atom(ast, TOK_MUL, ""),
                 zz_pair(ast, $1,
                 zz_pair(ast, $2,
                 NULL)));
            }
        | exp exp '/'     { 
            $$ = zz_pair(ast, zz_atom(ast, TOK_DIV, ""),
                 zz_pair(ast, $1,
                 zz_pair(ast, $2,
                 NULL)));
            }
        | exp exp '^'     { 
            $$ = zz_pair(ast, zz_atom(ast, TOK_EXP, ""),
                 zz_pair(ast, $1,
                 zz_pair(ast, $2,
                 NULL)));
            }
        | exp 'n'         {
            $$ = zz_pair(ast, zz_atom(ast, TOK_NEG, ""),
                 zz_pair(ast, $1,
                 NULL));
            }
        ;


