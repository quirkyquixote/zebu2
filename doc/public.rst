Public API
==========

.. default-domain:: c

AST
---

Handler for all nodes of the AST. Always handled by pointer, the actual node
may have more fields beyond the end of this struct; if so, the type determines
what the node can be cast to.

The type string is a pointer to a :type:`const char*` that is unique for that
node type, so two nodes are of the same type if the pointers---not the
strings---are the same.

All nodes are allocated by a :type:`zz_ast_mgr` object, that also keeps a
record of them and is responsible for deleting the ones that aren't used
anymore.

.. type:: struct zz_ast

   Base for all AST nodes

============================ ==============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_ast.type`       a const string, doubles as type name
---------------------------- ----------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_print()`           serialize a node
============================ ==============================================

.. member:: const char* zz_ast.type

   A const string, doubles as type name

.. function:: int zz_print(struct zz_ast* n, FILE* f)

   Serialize :data:`n`, and write the result to :data:`f`

AST Manager
-----------

Manager for the full AST

.. type:: struct zz_ast_mgr

   Manager for the full AST

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_ast_mgr.blobs`    all allocated nodes
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_ast_mgr_init()`      initialize AST manager
:func:`zz_ast_mgr_deinit()`    deinitialize AST manager
:func:`zz_ast_mgr_alloc()`     allocate node
:func:`zz_ast_mgr_gc()`        run garbage collector
============================== ============================================

.. member:: void* zz_ast_mgr.blobs

   All allocated nodes

.. function:: void zz_ast_mgr_init(struct zz_ast_mgr* a)

   Initialize AST manager.

.. function:: void zz_ast_mgr_deinit(struct zz_ast_mgr* a)

   Deinitialize AST manager.

.. function:: void* zz_ast_mgr_alloc(struct zz_ast_mgr* a, int size)

   Allocate a new and register it with the garbage collector.

.. function:: void zz_ast_mgr_gc(struct zz_ast_mgr* a, struct zz_ast* root)

   Destroy every allocated node that is not reachable from :data:`root`.

Pair
----

A node in a singly-linked list

This is very similar to a LISP cons cell: it holds pointers to a data
element (that can be another list) and the next element.

.. type:: struct zz_pair

   Node in a singly-linked list

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_pair.type`        always :data:`ZZ_PAIR`
:member:`~zz_pair.data`        pointer to the data of this cell
:member:`~zz_pair.next`        pointer to the next element
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_pair()`              create new pair
:func:`zz_is_pair()`           true if a node is a pair
:func:`zz_to_pair()`           cast node to pair
:func:`zz_foreach()`           iterate on a list of pairs
============================== ============================================

.. var:: const char* ZZ_PAIR

   Name and type of all list nodes.

.. member:: const char* zz_pair.type

   Always :data:`ZZ_PAIR`

.. member:: struct zz_ast* zz_pair.data

   Pointer to the data of this cell

.. member:: struct zz_ast* zz_pair.next

   Pointer to the next element

.. function:: struct zz_ast* zz_pair(struct zz_ast_mgr* a, \
              struct zz_ast* data, struct zz_ast* next)

   Create new pair

.. function:: int zz_is_pair(struct zz_ast* n)

   Return :data:`1` if :data:`n` is a pair, :data:`0` otherwise

.. function:: struct zz_pair* zz_to_pair(struct zz_ast* n)

   Return :data:`n` cast to :type:`zz_pair`, or :data:`NULL`.

.. function:: void zz_foreach(struct zz_ast* x, struct zz_ast* head)

   Iterate on a list of pairs.

Atom
----

An atomic element in the AST

Atoms have a token type and an associated string.

.. type:: struct zz_atom

   Leaf in the AST

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_atom.type`        never :data:`ZZ_PAIR`
:member:`~zz_atom.str`         string
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_atom()`              create new atom
:func:`zz_is_atom()`           true if a node is a atom
:func:`zz_to_atom()`           cast node to atom
============================== ============================================

.. member:: const char* zz_atom.type

   Anything but :data:`ZZ_PAIR`. Each user must provide their own token types.

.. member:: char[] zz_atom.str

   String for the atom

.. function:: struct zz_ast* zz_atom_with_len(struct zz_ast_mgr* a, \
             const char* type, const char* str, int len)
              struct zz_ast* zz_atom(struct zz_ast_mgr* a, \
              const char* type, const char* str)

   Construct new atom

.. function:: int zz_is_atom(struct zz_ast* n)

   Return :data:`1` if :data:`n` is an atom, :data:`0` otherwise

.. function:: struct zz_atom* zz_to_atom(struct zz_ast* n)

   Return :data:`n` cast to :type:`zz_atom`, or :data:`NULL`

