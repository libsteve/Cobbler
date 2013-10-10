#ifndef ITERATOR_H
#define ITERATOR_H

#include "Cobbler/primitive.h"

//////
// Iterator Interface

typedef primitive iterator;

extern iterator     *iter_get(void *);
extern iterator     *iter_rget(void *);
extern iterator     *iter_next(void *);
extern iterator     *iter_prev(void *);
extern primitive    *iter_value(void *);
extern iterator     *iter_delete(void *);
extern bool          iter_hasNext(void *);
extern bool          iter_hasPrev(void *);
extern bool          iter_isValid(void *);

#define VALUE(it)   iter_value(it)

#define foreach(it, iterable) \
    for (iterator *it = iter_get(iterable); it; it = iter_hasNext(it) ? iter_next(it) : NULL)

#define in ,
#define forin(value, iterable) \
    for (primitive *it = iter_get(iterable), *value = iter_value(it); \
         it; \
         it = iter_hasNext(it) ? iter_next(it) : NULL, value = iter_value(it))

//////
// Node Primitive

primitive_declare(node);
// primitive methods
extern node         *method(node, create, node *prev, primitive *value, node *next);
extern void          method(node, destroy);

// iterator methods
extern node         *method(node, iter_get);
extern node         *method(node, iter_rget);
extern node         *method(node, iter_next);
extern node         *method(node, iter_prev);
extern node         *method(node, iter_delete);
extern primitive    *method(node, iter_value);
extern bool          method(node, iter_hasNext);
extern bool          method(node, iter_hasPrev);
extern bool          method(node, iter_isValid);

// node methods
extern primitive    *method(node, value);
extern void          method(node, setValue, primitive *value);

// static node methods
extern node         *method(node, next);
extern void          method(node, setNext, node *next);
extern node         *method(node, prev);
extern node         *method(node, setPrev, node *prev);

primitive_define(node, primitive, {
        primitive *value;
        struct node *next;
        struct node *prev;
    },
    // primitive methods
    virtual(node, create),
    virtual(node, destroy),

    // iterator methods
    virtual(node, iter_get),
    virtual(node, iter_rget),
    virtual(node, iter_next),
    virtual(node, iter_prev),
    virtual(node, iter_delete),
    virtual(node, iter_value),
    virtual(node, iter_hasNext),
    virtual(node, iter_hasPrev),
    virtual(node, iter_isValid),

    //node methods
    virtual(node, value),
    virtual(node, setValue)
);

#endif