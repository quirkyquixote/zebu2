API Doc
=======

.. default-domain:: c
.. highlight:: c

AST Type
--------

Defines an AST node that holds data of a specific type; contains pointers to
functions to manipulate such data.

.. type:: struct zz_type

   Defines a type of AST node

============================ ==============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_type.serialize` prints to :type:`FILE`
:member:`~zz_type.copy`      clones a node
:member:`~zz_type.cmp`       compares two nodes
---------------------------- ----------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_pair_type()`       returns type for pair nodes
:func:`zz_int_type()`        returns type for atom nodes holding integes
:func:`zz_ptr_type()`        returns type for atom nodes holding pointers
:func:`zz_str_type()`        returns type for atom nodes holding strings
============================ ==============================================

.. member:: int zz_type.serialize(struct zz_ast* a, FILE* f)

   Print :data:`a` to :data:`f`, return number of characters.

.. member:: struct zz_ast* zz_type.copy(struct zz_ast* a)

   Creates a copy of :data:`a`; if :data:`a` is a pair, its head and tail are
   copied recursively.

.. member:: int zz_type.cmp(struct zz_ast* a, struct zz_ast* b)

   Returns a number that is equal, less, or greater than zero if :data:`a` is
   equal, less, or greater than :data:`b`, respectively; assumes :data:`a` and
   :data:`b` to be of the same type.

.. member:: int zz_type.serialize(struct zz_ast* a, FILE* f)

   Print :data:`a` to :data:`f`, return number of characters.

.. function:: const struct zz_type* zz_pair_type(void)

   Return type for pair nodes.

.. function:: const struct zz_type* zz_int_type(void)

   Return type for atomic nodes holding integers.

.. function:: const struct zz_type* zz_ptr_type(void)

   Return type for atomic nodes holding pointers.

.. function:: const struct zz_type* zz_str_type(void)

   Return type for atomic nodes holding strings.

AST
---

Handler for all nodes of the AST. Usually, no actual instances of
:type:`zz_ast` exist, but a number of other types (:type:`zz_int`,
:type:`zz_ptr`, :type:`zz_str`, :type:`zz_pair`) share the same first field, so
they can be handled through pointers to this type; see :func:`zz_cast()`.

The :member:`~zz_ast.type` field determines what the actual object is and how
the data may be manipulated;

All nodes are allocated with the Boehms Garbage Collector.

.. type:: struct zz_ast

   Base for all AST nodes

============================ ==============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_ast.type`       pointer to a :type:`zz_type`
---------------------------- ----------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_typeof()`          get type of node
:func:`zz_cast()`            cast node to type, or return NULL
:func:`zz_print()`           serialize a node
:func:`zz_copy()`            copy recursively
:func:`zz_cmp()`             compare two nodes
============================ ==============================================

.. member:: const struct zz_type* zz_ast.type

   Determines the type of the node.

.. function:: const struct zz_type* zz_typeof(struct zz_ast* a)

   Return type of node; if :data:`a` is :data:`NULL`, return :data:`NULL`.

.. function:: TYPE zz_cast(TYPE, struct zz_ast* a)

   If :data:`a` is of type :func:`TYPE_type()`, return :data:`a` cast to
   :type:`struct TYPE`, else return :data:`NULL`.
   
   This is implemented as a macro that depends on the names of
   :func:`TYPE_type()` and :type:`struct TYPE` matching.

.. function:: int zz_print(struct zz_ast* n, FILE* f)

   Serialize :data:`n`, and write the result to :data:`f`

.. function:: struct zz_ast* zz_copy(struct zz_ast* n)

   Creates a copy of :data:`a`; if :data:`a` is a pair, its head and tail are
   copied recursively.

.. function:: int zz_cmp(struct zz_ast* a, struct zz_ast* b)

   Returns a number that is equal, less, or greater than zero if :data:`a` is
   equal, less, or greater than :data:`b`, respectively.
   The comparison is performed in two steps: first, the types of both nodes are
   compared, and only if they are equal, is the comparison function for that
   type called for the nodes.

Pair
----

A node in a singly-linked list

This is basically a LISP cons cell: it holds pointers to a data element (the
head, that can be another list) and the next element (the tail).

.. type:: struct zz_pair

   Node in a singly-linked list

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_pair.type`        always :func:`zz_pair_type()`
:member:`~zz_pair.head`        pointer to the data of this cell
:member:`~zz_pair.tail`        pointer to the next element
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_pair()`              create new pair
:func:`zz_head()`              return head
:func:`zz_tail()`              return tail
:func:`zz_insert()`            insert element after
:func:`zz_replace()`           replace head
:func:`zz_foreach()`           iterate on a list of pairs
:func:`zz_unpack()`            extract all list elements
:func:`zz_index()`             return nth element
============================== ============================================

.. member:: const struct zz_type* zz_pair.type

   Always :func:`zz_pair_type()`.

.. member:: struct zz_ast* zz_pair.head

   Pointer to the data of this cell

.. member:: struct zz_ast* zz_pair.tail

   Pointer to the next element

.. function:: struct zz_ast* zz_pair(struct zz_ast* head, struct zz_ast* tail)

   Create new pair

.. function:: struct zz_ast* zz_head(struct zz_ast* a)

   Return head if :data:`a` is pair, :data:`NULL` otherwise.

.. function:: struct zz_ast* zz_tail(struct zz_ast* a)

   Return tail if :data:`a` is pair, :data:`NULL` otherwise.

.. function:: struct zz_ast* zz_insert(struct zz_ast* a, struct zz_ast* next)

   Insert :data:`next` after :data:`a`; returns newly cleated :type:`zz_pair`
   object whose head is :data:`next`.

.. function:: void zz_replace(struct zz_ast* a, struct zz_ast* head)

   Replace head of :data:`a` with :data:`head`.

.. function:: void zz_foreach(struct zz_ast* x, struct zz_ast* head)

   Iterate on a list of pairs.

.. function:: int zz_unpack(struct zz_ast* a, ...)

   Extract all elements from a list. Every argument after the first one must be
   a pointer to :type:`struct zz_ast*` where the element can be stored.
   Implemented as a macro that appends :data:`NULL` to the element list and
   calls the actual function.

.. function:: struct zz_ast* zz_index(struct zz_ast* a, int i)

   Return :data:`i`-th element of the list whose handler is :data:`a`.
   If :data:`a` is not a list, :data:`zz_index(a, 0)` returns :data:`a`, and
   :data:`zz_index(a, N)` for every other :data:`N` returns :data:`NULL`.
   For every :data:`i` past the end of the list, :data:`NULL` is returned.
   Append an element to an existing list.

Integer
-------

Integers hold data of type :type:`int`.

.. type:: struct zz_int

   Leaf in the AST

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_int.type`         always :func:`zz_int_type()`
:member:`~zz_int.num`          integer
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_int()`               create new integer
============================== ============================================

.. member:: const struct zz_type* zz_int.type

   Always :func:`zz_int_type()`.

.. member:: int zz_int.num

   Integer data

.. function:: struct zz_ast* zz_int(int num)

   Construct new integer

Pointer
-------

Pointers hold data of type :type:`void*`

.. type:: struct zz_ptr

   Leaf in the AST

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_ptr.type`         always :func:`zz_ptr_type()`
:member:`~zz_ptr.ptr`          pointer
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_ptr()`               create new pointer
============================== ============================================

.. member:: const struct zz_type* zz_ptr.type

   Always :func:`zz_ptr_type()`.

.. member:: void* zz_ptr.ptr

   Pointer data

.. function:: struct zz_ast* zz_ptr(void* ptr)

   Conptruct new pointer

String
------

Strings hold a null-terminated byte array.

.. type:: struct zz_str

   Leaf in the AST

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_str.type`         always :func:`zz_str_type()`
:member:`~zz_str.str`          string
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_str()`               create new string
============================== ============================================

.. member:: const struct zz_type* zz_str.type

   Always :func:`zz_str_type()`.

.. member:: char[] zz_str.str

   Null-terminated byte array.

.. function:: struct zz_ast* zz_str_with_len(const char* str, int len)
              struct zz_ast* zz_str(const char* str)

   Construct new string

List
----

Utilities to build lists.

.. type:: struct zz_list

============================== ============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_list.first`       pointer to the first element
:member:`~zz_list.last`        pointer to the last element
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_list()`              create list
:func:`zz_append()`            append element to list
:func:`zz_prepend()`           prepend element to list
============================== ============================================

.. member:: struct zz_ast* zz_list.first

   Pointer to the first element of a list, or NULL.

.. member:: struct zz_ast* zz_list.last

   Pointer to the last element of a list, or NULL.

.. function:: struct zz_list zz_list(struct zz_ast* first, ...)

   Construct list from elements list.
   Implemented as a macro that appends :data:`NULL` to the element list and
   calls the actual function.
   To create an empty list, pass :data:`NULL` as the only element.

.. function:: struct zz_list zz_append(struct zz_list l, struct zz_ast* a)

   Append an element to an existing list.

.. function:: struct zz_list zz_prepend(struct zz_list l, struct zz_ast* a)

   Prepend an element to an existing list.

