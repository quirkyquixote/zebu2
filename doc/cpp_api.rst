C++ API
=======

.. default-domain:: cpp
.. highlight:: cpp

Ast
---

.. class:: Ast : public gc

   Base class for all AST nodes, always handled by pointer, and garbage
   collected by default.

=============================== ==============================================
Member functions
=============================== ==============================================
:func:`~Ast::~Ast()`            destructor
:func:`~Ast::operator==()`      equality comparation
:func:`~Ast::operator!=()`      equality comparation
:func:`~Ast::operator<()`       relational comparation
:func:`~Ast::operator>()`       relational comparation
:func:`~Ast::operator<=()`      relational comparation
:func:`~Ast::operator>=()`      relational comparation
:func:`~Ast::serialize()`       serialization
:func:`~Ast::type()`            return type
------------------------------- ----------------------------------------------
**Related functions**
------------------------------- ----------------------------------------------
:func:`make_ast()`              create Ast object
:func:`make_pair()`             create Ast object
:func:`make_list()`             create Ast object
:func:`ast_get()`               type-safe access
:func:`head()`                  type-safe access
:func:`tail()`                  type-safe access
:func:`begin()`                 return iterator
:func:`end()`                   return iterator
:func:`operator<<()`            serialization
:func:`remove_if()`             remove elements
:func:`remove()`                remove elements
=============================== ==============================================

.. function:: Ast::~Ast()

   Destructor

.. function:: virtual bool Ast::operator==(const Ast& other) const = 0
        virtual bool Ast::operator==(const Ast& other) const = 0
        virtual bool Ast::operator!=(const Ast& other) const = 0
        virtual bool Ast::operator<(const Ast& other) const = 0
        virtual bool Ast::operator>(const Ast& other) const = 0
        virtual bool Ast::operator<=(const Ast& other) const = 0
        virtual bool Ast::operator>=(const Ast& other) const = 0

   Comparator

.. function:: virtual void Ast::serialize(std::ostream& os) const = 0

   Serialize

.. function:: virtual const std::type_info& Ast::type() const noexcept = 0

   Return type

.. function:: template<typename T> Ast* make_ast(const T& value)
        template<typename T> Ast* make_ast(T&& value)

   Creates an :type:`Ast` object

.. function:: Ast* make_pair(Ast* head, Ast* tail)

   Creates an :type:`Ast` object with :type:`std::pair\<Ast*, Ast*>` data

.. function:: Ast* make_list()
        template<typename ...Args> Ast* make_list(Ast* head, Args... args)

   Creates a linked list of :type:`Ast` objects with :type:`std::pair\<Ast*, Ast*>`
   data, holding the arguments passed to the function.

.. function:: template<typename T> inline T& ast_get(Ast* ast)
                template<typename T> inline T ast_get(const Ast* ast)

   Type-safe access to the contained object

.. function:: const Ast* head(const Ast* ast)
                Ast* head(Ast* ast)

   Type-safe access

.. function:: const Ast* tail(const Ast* ast)
                Ast* tail(Ast* ast)

   Type-safe access

.. function:: Ast_iterator begin(Ast* ptr)
        Ast_const_iterator begin(const Ast* ptr)

   Return iterator to the beginning of an :type:`Ast` list

.. function:: Ast_iterator end(Ast* ptr)
        Ast_const_iterator end(const Ast* ptr)

   Return iterator to the end of an :type:`Ast` list

.. function:: std::ostream& operator<<(std::ostream& os, const std::pair<Ast*, Ast*>& p)

   Actually does nothing. Serialization of pairs is handled by the generic
   :type:`Ast` serializer.

.. function:: std::ostream& operator<<(std::ostream& os, const Ast* ast)

   Serializes :type:`Ast` node, and its children.

.. function:: template<typename Pred> Ast* remove_if(Ast* l, Pred pred)

   Removes all nodes whose head satisfied :var:`pred` from the list whose first
   element is :var:`l`.

.. function:: Ast* remove(Ast* l, Ast* x)

   Removes all nodes whose head is equal to :var:`x` from the list whose first
   element is :var:`l`.


Ast_impl
--------

   Actual implementation of a node, with data.

.. class:: class Ast_impl : public Ast

=============================== ==============================================
Member functions
=============================== ==============================================
:func:`~Ast_impl::Ast_impl()`   constructor
:func:`~Ast_impl::~Ast_impl()`  destructor
:func:`~Ast_impl::operator==()` equality comparation
:func:`~Ast_impl::operator!=()` equality comparation
:func:`~Ast_impl::operator<()`  relational comparation
:func:`~Ast_impl::operator>()`  relational comparation
:func:`~Ast_impl::operator<=()` relational comparation
:func:`~Ast_impl::operator>=()` relational comparation
:func:`~Ast_impl::serialize()`  serialization
:func:`~Ast_impl::type()`       return type
=============================== ==============================================

.. function::  Ast_impl::Ast_impl(const T& data)
         Ast_impl::Ast_impl(T&& data)

   Constructors

.. function::  Ast_impl::~Ast_impl()

   Destructor

.. function::  bool Ast_impl::operator==(const Ast& other) const
         bool Ast_impl::operator==(const Ast& other) const
         bool Ast_impl::operator!=(const Ast& other) const
         bool Ast_impl::operator<(const Ast& other) const
         bool Ast_impl::operator>(const Ast& other) const
         bool Ast_impl::operator<=(const Ast& other) const
         bool Ast_impl::operator>=(const Ast& other) const

   Comparators

.. function:: void Ast_impl::serialize(std::ostream& os) const

   Serializer

.. function:: const std::type_info& Ast_impl::type() const noexcept

   Return type

Ast_iterator
------------

   Allows treating linked lists of :type:`Ast` holding
   :type:`std::pair\<Ast*, Ast*>` objects as STL sequences.

.. class:: Ast_iterator

=============================================== ==============================
Member types
=============================================== ==============================
:type:`~Ast_iterator::value_type`               value
:type:`~Ast_iterator::difference_type`          difference 
:type:`~Ast_iterator::pointer`                  pointer to value
:type:`~Ast_iterator::reference`                reference to value
:type:`~Ast_iterator::iterator_category`        type of iterator
----------------------------------------------- ------------------------------
**Member functions**
----------------------------------------------- ------------------------------
:func:`~Ast_iterator::Ast_iterator()`           constructors
:func:`~Ast_iterator::~Ast_iterator()`          destructor
:func:`~Ast_iterator::operator=()`              assignment
:func:`~Ast_iterator::operator*()`              dereference
:func:`~Ast_iterator::operator++()`             increment
:func:`~Ast_iterator::operator==()`             comparison
:func:`~Ast_iterator::operator!=()`             comparison
=============================================== ==============================

.. type:: typedef Ast* Ast_iterator::value_type

   Value held by the iterator

.. type:: typedef ptrdiff_t Ast_iterator::difference_type

   Type of difference between two iterators

.. type:: typedef Ast** Ast_iterator::pointer

   Pointer to iterator value

.. type:: typedef Ast*& Ast_iterator::reference

   Reference to iterator value

.. type:: typedef std::forward_iterator_tag Ast_iterator::iterator_category

   Always forward iterator


.. function:: Ast_iterator::Ast_iterator()
        Ast_iterator::Ast_iterator(Ast* ptr)
        Ast_iterator::Ast_iterator(const Ast_iterator& other)
        Ast_iterator::Ast_iterator(Ast_iterator&& other)

   Constructors

.. function:: Ast_iterator::~Ast_iterator()

   Destructor

.. function:: Ast_iterator& Ast_iterator::operator=(const Ast_iterator& other)
        Ast_iterator& Ast_iterator::operator=(Ast_iterator&& other)

   Assignment operators

.. function:: Ast*& Ast_iterator::operator*()

   Dereference operator

.. function:: Ast_iterator& Ast_iterator::operator++()
        Ast_iterator Ast_iterator::operator++(int)

   Pre- and post-increment operators

.. function:: bool Ast_iterator::operator==(const Ast_iterator& other) const
        bool Ast_iterator::operator!=(const Ast_iterator& other) const

   Equality comparators.

Ast_const_iterator
------------

   Allows treating linked lists of :type:`Ast` holding
   :type:`std::pair\<Ast*, Ast*>` objects as STL sequences.

.. class:: Ast_const_iterator

===================================================== ========================
Member types                                         
===================================================== ========================
:type:`~Ast_const_iterator::value_type`               value
:type:`~Ast_const_iterator::difference_type`          difference
:type:`~Ast_const_iterator::pointer`                  pointer to value
:type:`~Ast_const_iterator::reference`                reference to value
:type:`~Ast_const_iterator::iterator_category`        type of iterator
----------------------------------------------------- ------------------------
**Member functions**
----------------------------------------------------- ------------------------
:func:`~Ast_const_iterator::Ast_const_iterator()`     constructors
:func:`~Ast_const_iterator::~Ast_const_iterator()`    destructor
:func:`~Ast_const_iterator::operator=()`              assignment
:func:`~Ast_const_iterator::operator*()`              dereference
:func:`~Ast_const_iterator::operator++()`             increment
:func:`~Ast_const_iterator::operator==()`             comparison
:func:`~Ast_const_iterator::operator!=()`             comparison
===================================================== ========================

.. type:: typedef Ast* Ast_const_iterator::value_type

   Value held by the iterator

.. type:: typedef ptrdiff_t Ast_const_iterator::difference_type

   Type of difference between two iterators

.. type:: typedef Ast** Ast_const_iterator::pointer

   Pointer to iterator value

.. type:: typedef Ast*& Ast_const_iterator::reference

   Reference to iterator value

.. type:: typedef std::forward_iterator_tag Ast_const_iterator::iterator_category

   Always forward iterator


.. function:: Ast_const_iterator::Ast_const_iterator()
        Ast_const_iterator::Ast_const_iterator(Ast* ptr)
        Ast_const_iterator::Ast_const_iterator(const Ast_const_iterator& other)
        Ast_const_iterator::Ast_const_iterator(Ast_const_iterator&& other)

   Constructors

.. function:: Ast_const_iterator::~Ast_const_iterator()

   Destructor

.. function:: Ast_const_iterator& Ast_const_iterator::operator=(const Ast_const_iterator& other)
        Ast_const_iterator& Ast_const_iterator::operator=(Ast_const_iterator&& other)

   Assignment operators

.. function:: Ast*& Ast_const_iterator::operator*()

   Dereference operator

.. function:: Ast_const_iterator& Ast_const_iterator::operator++()
        Ast_const_iterator Ast_const_iterator::operator++(int)

   Pre- and post-increment operators

.. function:: bool Ast_const_iterator::operator==(const Ast_const_iterator& other) const
        bool Ast_const_iterator::operator!=(const Ast_const_iterator& other) const

   Equality comparators.


