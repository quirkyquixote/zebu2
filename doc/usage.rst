Usage
=====

.. default-domain:: c
.. highlight:: c

Custom Node Types
-----------------

Zebu allows declaring new custom node types. To do so, the user needs three
elements

- a structure that holds the data :type:`struct TYPE`,
- a function :func:`CUSTOM_type()` that returns a :type:`zz_type` object
- optionally, one or more functions to construct new :type:`struct TYPE` objects.

The first field of :type:`struct TYPE` must be a const pointer to an object of
type :type:`zz_type`, to match with :type:`zz_ast` objects::

    struct CUSTOM {
        const struct zz_type *type;
        /* further fields */
    };

The :data:`type` field of this struct will always hold a pointer to the object
returned by the :func:`CUSTOM_type()` function::

    const struct zz_type* CUSTOM_type(void)
    {
        static struct zz_type *type = {
            .serialize = serialize_CUSTOM
        };
        return &type;
    }

    int serialize_CUSTOM(struct zz_ast *a, FILE *f)
    {
        /* implement CUSTOM node serialization */
    }

    /* other functions */

The :type:`zz_type` object does not only identify the type of the nodes, but
holds pointers to functions to perform operations on nodes of type
:type:`CUSTOM`.

Finally, you should declare one or more constructors for objects of this type.
This is not actually required, but makes your life easier::

    struct zz_ast* CUSTOM(/* arguments */)
    {
        struct CUSTOM* a = GC_malloc_atomic(sizeof(*a));
        a->type = CUSTOM_type();
        /* initialize fields of a from arguments */
        return (void *)a;
    }

See :type:`zz_str` for an example of multiple constructors.

See examples/shon.y for an example of a custom node type at work; it declares a
custom function type for :type:`struct zz_ast*(struct zz_ast*)`.

