#ifndef LIST_H
#define LIST_H

#include "Cobbler/primitive.h"
#include "Cobbler/iterator.h"

//////
// List Primitive

primitive_declare(list);

// primitive methods
extern list      *method(list, create);
extern void       method(list, destroy);

// iterator interface override methods
extern iterator  *method(list, iter_get);
extern iterator  *method(list, iter_rget);
extern bool       method(list, iter_isValid);

// list methods
extern primitive *method(list, head);
extern primitive *method(list, tail);

extern list      *method(list,  push, primitive *p);
extern list      *method(list, rpush, primitive *p);

extern primitive *method(list,  peek);
extern primitive *method(list, rpeek);

extern list      *method(list,  pop, primitive *output p);
extern list      *method(list, rpop, primitive *output p);

primitive_define(list, node, {
        size_t length;
    },
    // primitive methods
    virtual(list, create),
    virtual(list, destroy),

    // iterator interface override methods
    virtual(list, iter_get),
    virtual(list, iter_rget),
    virtual(list, iter_isValid),

    // list methods
    virtual(list, head),
    virtual(list, tail),
    virtual(list,  push),
    virtual(list, rpush),
    virtual(list,  peek),
    virtual(list, rpeek),
    virtual(list,  pop),
    virtual(list, rpop)
);

#endif
