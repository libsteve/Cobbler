#ifndef ITERATOR_H
#define ITERATOR_H

#include "Cobbler/primitive.h"

//////
// Iterator Interface

typedef primitive iterator;

extern iterator     *iter_get(iterator *);
extern iterator     *iter_next(iterator *);
extern iterator     *iter_prev(iterator *);
extern primitive    *iter_value(iterator *);
extern iterator     *iter_delete(iterator *);
extern bool          iter_hasNext(iterator *);
extern bool          iter_hasPrev(iterator *);
extern bool          iter_isValid(iterator *);

#define VALUE(it)   iter_value(it)

#define foreach(it, iterable) \
    for (iterator *it = iter_get(iterable) *__##it = it; \
         it && iter_hasNext(it) && iter_isValid(it); \
         __##it == it ? it = iter_next(it) : __##it = it)

#define in ,
#define forin(value, iterable) \
    for (primitive *it = iter_get(iterable), *value = iter_value(it); \
         it && iter_hasNext(it); \
         it = iter_next(it), value = iter_value(it))

//////
// Node Primitive

primitive_declare(node);
// primitive methods
extern node         *method(node, create, node *prev, primitive *value, node *next);
extern void          method(node, destroy);

// iterator methods
extern node         *method(node, iter_get);
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
    virtual(node, iter_next),
    virtual(node, iter_prev),
    virtual(node, iter_remove),
    virtual(node, iter_value),
    virtual(node, iter_hasNext),
    virtual(node, iter_hasPrev),
    virtual(node, iter_isValid),

    //node methods
    virtual(node, value),
    virtual(node, setValue)
);

#endif