#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//////
// definitions for primitive data structures

struct primitive;
struct primitive_class;
struct virtual_method;

typedef struct primitive       primitive;           // primitive
typedef struct primitive_class primitive_class;     // primitive_class
typedef struct virtual_method  virtual_method;      // virtual_method
typedef void *(*virtual_method_fn)(void *, void *, const char *, ...);  // virtual method function pointer

struct primitive_class {
    size_t      size;                          // how much size the primitive struct takes up
    const char  *primitive_type;                // a string name of the primitive structure
    size_t      method_count;                  // a count of the amount of virtual methods
    size_t      ownership_count;               // the current reference count
    struct virtual_method   *methods;          // an array of the virtual methods for the primitive
    struct primitive_class  *super_primitive;  // a pointer to the 'super class' of the primitive
};

#define inherit_primitive(super_primitive) super_primitive primitive
struct primitive {
    inherit_primitive(primitive_class);
};

struct virtual_method {
    const char        *signature;       // a string representation for the method
    virtual_method_fn functionpointer;  // a function pointer to the method implemenation
};

//////
// defining custom primitive structures

#define PrimitiveClass(p)                       p ## _PrimitiveClass ()
#define PrimitiveCast(new_primitive, instance)  ((new_primitive *)instance)
#define PrimitiveName(p)                       #p

#define method_name(primitive_name, fn)         primitive_name ## _ ## fn
#define method(primitive_name, fn, ...)         method_name(primitive_name, fn) (primitive_name *this , primitive_class *this_class , const char *this_method , ## __VA_ARGS__)

#define using_custom_virtual(fn, fn_name)       (struct virtual_method){ .signature = #fn_name , .functionpointer = (virtual_method_fn)& fn }
#define using_virtual(primitive_name, fn)       using_custom_virtual(method_name(primitive_name, fn), fn)

#define primitive_declare(primitive_name)       struct primitive_name; typedef struct primitive_name primitive_name;
#define primitive_define(primitive_name, super_primitive_name, structure, ...) \
    typedef struct primitive_name { \
        inherit_primitive(super_primitive_name); \
        struct structure; \
    } primitive_name; \
    static inline primitive_class * PrimitiveClass(primitive_name) { \
        static bool setup = true; \
        static primitive_class c; \
        static virtual_method methods[] = { __VA_ARGS__ }; \
        if (setup) { \
            setup = false; \
            c = (struct primitive_class){ \
                .size = sizeof(struct primitive_name), \
                .primitive_type = PrimitiveName(primitive_name), \
                .method_count = sizeof(methods) / sizeof(struct virtual_method), \
                .methods = methods, \
                .super_primitive = PrimitiveClass(super_primitive_name)}; \
        } \
        return &c; \
    };

//////
// interacting with primitive structures

#include <stdio.h>

static inline virtual_method_fn virtual_method_lookup(primitive_class *c, const char *fn) {
    // printf("searching for %s in class %s\n", fn, c->primitive_type);
    for (int i = 0; i < c->method_count; i++) {
        if (strcmp(c->methods[i].signature, fn) == 0) {
            return c->methods[i].functionpointer;
        }
    }
    if (c->super_primitive != NULL) {
        return virtual_method_lookup(c->super_primitive, fn);
    }
    return (virtual_method_fn)0;
}

#define PrimitiveType(instance)     (PrimitiveCast(primitive_class, instance)->primitive_type)
#define SuperPrimitive(instance)    (PrimitiveCast(primitive_class, instance)->super_primitive)

#define super_virtual_call(returns, ...)                ((returns (*)(void *, void *, const char *, ...)) virtual_method_lookup(this_class->super_primitive, this_method)) (this, this_class->super_primitive, this_method, ## __VA_ARGS__)
#define virtual_call(returns, fn, instance, ...)        ((returns (*)(void *, void *, const char *, ...)) virtual_method_lookup((primitive_class *)instance, #fn)) (instance, instance, #fn, ## __VA_ARGS__)
#define static_call(primitive_name, fn, instance, ...)  method_name(primitive_name, fn) ((primitive_name *)instance, (primitive_class *)instance, #fn, ## __VA_ARGS__)

#define SuperCreate(...)    super_virtual_call(primitive *, ## __VA_ARGS__)
#define SuperCopy()         super_virtual_call(primitive *)
#define SuperDestroy()      super_virtual_call(void)

#define output *

//////
// creating primitive structure instances

extern primitive *__create_instance(primitive_class *c);
#define create(primitive_name, ...)         virtual_call(primitive_name *, create, __create_instance(PrimitiveClass(primitive_name)), ## __VA_ARGS__)
#define autocreate(primitive_name, ...)     autodisown(create(primitive_name, ## __VA_ARGS__))
#define auto(primitive_name, ...)           autocreate(primitive_name, ## __VA_ARGS__)
extern void *copy(void *);
extern void destroy(void *);

//////
// default primitive structures

extern primitive *method(primitive, create);
extern primitive *method(primitive, copy);
extern void       method(primitive, destroy);

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

static inline primitive_class * PrimitiveClass(primitive) {
    static bool setup = true;
    static primitive_class c;
    static virtual_method methods[] = {
        using_virtual(primitive, create),
        using_virtual(primitive, copy),
        using_virtual(primitive, destroy),
        using_custom_virtual(own, own),
        using_custom_virtual(disown, disown),
        using_custom_virtual(autodisown, autodisown)
    };
    if (setup) {
        setup = false;
        c = (primitive_class){
            .size = sizeof(struct primitive),
            .primitive_type = PrimitiveName(primitive),
            .method_count = sizeof(methods) / sizeof(struct virtual_method),
            .methods = methods,
            .super_primitive = NULL};
    }
    return &c;
}

//////
// reference counting structures and implementations

#define context for(autodisown_pool *__p__ = create(autodisown_pool), \
                    *__i__ = (void *)1; __i__ != NULL; \
                    __i__ = NULL, __p__ = disown(__p__))

primitive_declare(autodisown_pool);

extern autodisown_pool *method(autodisown_pool, create);
extern void             method(autodisown_pool, destroy);

primitive_define(autodisown_pool, primitive, {
        struct autodisown_pool *previous_pool;
        struct primitive **autodisowned_objects;
    },
    using_virtual(autodisown_pool, create), 
    using_virtual(autodisown_pool, destroy));

#endif
