API Doc
=======

.. default-domain:: c

AST Type
--------

Defines a type of AST with an unique associated data; holds pointers to
functions to manipulate such data.

.. type:: struct zz_type

   Defines a type of AST node

============================ ==============================================
**Member fields**
---------------------------------------------------------------------------
:member:`~zz_type.serialize` prints to :type:`FILE`
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

Handler for all nodes of the AST. Always handled by pointer, the actual node
may have more fields beyond the end of this struct; if so, the type determines
what the node can be cast to; see :func:`zz_to_pair()`, :func:`zz_to_int()`,
:func:`zz_to_ptr()`, and :func:`zz_to_str()`.

The type field poinst of a :type:`const struct zz_type` that determines the
actual node payload, and how to handle it.

All nodes are allocated with the Boehms Garbage Collector so the user doesn't
need to keep track of them.

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

.. member:: const struct zz_type* zz_ast.type

   A const string, doubles as type name

.. function:: int zz_print(struct zz_ast* n, FILE* f)

   Serialize :data:`n`, and write the result to :data:`f`

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
:member:`~zz_pair.type`        always :func:`zz_pair_type()`
:member:`~zz_pair.head`        pointer to the data of this cell
:member:`~zz_pair.tail`        pointer to the next element
------------------------------ --------------------------------------------
**Related functions**
---------------------------------------------------------------------------
:func:`zz_pair()`              create new pair
:func:`zz_is_pair()`           true if a node is a pair
:func:`zz_to_pair()`           cast node to pair
:func:`zz_head()`              return head
:func:`zz_tail()`              return tail
:func:`zz_foreach()`           iterate on a list of pairs
============================== ============================================

.. member:: const struct zz_type* zz_pair.type

   Always :func:`zz_pair_type()`.

.. member:: struct zz_ast* zz_pair.head

   Pointer to the data of this cell

.. member:: struct zz_ast* zz_pair.tail

   Pointer to the next element

.. function:: struct zz_ast* zz_pair(struct zz_ast* head, struct zz_ast* tail)

   Create new pair

.. function:: int zz_is_pair(struct zz_ast* n)

   Return :data:`1` if :data:`n` is a pair, :data:`0` otherwise

.. function:: struct zz_pair* zz_to_pair(struct zz_ast* n)

   Return :data:`n` cast to :type:`zz_pair`, or :data:`NULL`.

.. function:: struct zz_ast* zz_head(struct zz_ast* a)

   Return head if :data:`a` is pair, :data:`NULL` otherwise.

.. function:: struct zz_ast* zz_tail(struct zz_ast* a)

   Return tail if :data:`a` is pair, :data:`NULL` otherwise.

.. function:: void zz_foreach(struct zz_ast* x, struct zz_ast* head)

   Iterate on a list of pairs.

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
:func:`zz_is_int()`            true if a node is a integer
:func:`zz_to_int()`            cast node to integer
============================== ============================================

.. member:: const struct zz_type* zz_int.type

   Always :func:`zz_int_type()`.

.. member:: int zz_int.num

   Integer data

.. function:: struct zz_ast* zz_int(int num)

   Construct new integer

.. function:: int zz_is_int(struct zz_ast* n)

   Return :data:`1` if :data:`n` is an integer, :data:`0` otherwise

.. function:: struct zz_int* zz_to_int(struct zz_ast* n)

   Return :data:`n` cast to :type:`zz_int`, or :data:`NULL`

Pointer
-------

Pointers hold data of type :type:`void*`

.. type:: ptruct zz_ptr

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
:func:`zz_is_ptr()`            true if a node is a pointer
:func:`zz_to_ptr()`            cast node to pointer
============================== ============================================

.. member:: const struct zz_type* zz_ptr.type

   Always :func:`zz_ptr_type()`.

.. member:: void* zz_ptr.ptr

   Pointer data

.. function:: struct zz_ast* zz_ptr(void* ptr)

   Conptruct new pointer

.. function:: int zz_is_ptr(ptruct zz_ast* n)

   Return :data:`1` if :data:`n` is an pointer, :data:`0` otherwise

.. function:: ptruct zz_ptr* zz_to_ptr(ptruct zz_ast* n)

   Return :data:`n` cast to :type:`zz_ptr`, or :data:`NULL`

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
:func:`zz_is_str()`            true if a node is a string
:func:`zz_to_str()`            cast node to string
============================== ============================================

.. member:: const struct zz_type* zz_str.type

   Always :func:`zz_str_type()`.

.. member:: char[] zz_str.str

   Null-terminated byte array.

.. function:: struct zz_ast* zz_str_with_len(const char* str, int len)
              struct zz_ast* zz_str(const char* str)

   Construct new string

.. function:: int zz_is_str(struct zz_ast* n)

   Return :data:`1` if :data:`n` is an string, :data:`0` otherwise

.. function:: struct zz_str* zz_to_str(struct zz_ast* n)

   Return :data:`n` cast to :type:`zz_str`, or :data:`NULL`

