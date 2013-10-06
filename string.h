#ifndef string_H
#define string_H

#include "primitive.h"

primitive_declare(string);

extern string  *method(string, create, const char *);
extern void     method(string, destroy);
extern string  *method(string, copy);

extern char     method(string, charAt, unsigned int index);
extern bool     method(string, isEqual, string *other);

extern const char *method(string, c_string);

primitive_define(string, primitive, {
        char *str;
        size_t length;
    },
    using_virtual(string, create),
    using_virtual(string, destroy),
    using_virtual(string, copy));

#define PSTRING(__string) autodisown(create(string, __string))

#endif
