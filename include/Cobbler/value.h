#ifndef VALUE_H
#define VALUE_H

#include "primitive.h"
#include "string.h"

primitive_declare(value);

extern void    method(value, destroy);
extern value  *method(value, copy);

primitive_define(value, primitive, {
        void *bytes;
        size_t size;
        string *type;
    }, 
    virtual(value, destroy),
    virtual(value, copy));

extern value    *method(value, initialize,          void *bytes, size_t size);
extern value    *method(value, initializeWithType,  void *bytes, size_t size, string *type);
extern void     *method(value, value,               size_t output size);
extern string   *method(value, type);

#endif
