#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

struct primative_class;
struct primative;

typedef struct primative       primative;           // primative
typedef struct primative_class primative_class;     // primative_class

typedef primative *(*init_fn)(primative *, ...);	// init_fn
typedef void (*destroy_fn)(primative *);          	// destroy_fn
typedef primative *(*copy_fn)(primative *);         // copy_fn

struct primative_class {
	size_t      size;
	init_fn     initialize;
	destroy_fn  destroy;
	copy_fn     copy;
	size_t      ownership_count;
	struct primative_class *super_primative;
};

struct primative {
	primative_class primative;
};

extern primative_class primative_PrimativeClass;

#define PrimativeClass(p) (*(primative_class *)(p))
#define PrimativeClassForPrimative(p) p ## _PrimativeClass
#define inherit_primative(super_primative) super_primative primative
#define SuperPrimativeClass(p) (*(PrimativeClass(p).super_primative))

/* macros for declaring the existance and structure of primatives (for .h file) */

#define define_primative_begin(primative_name, super_primative) extern primative_class PrimativeClassForPrimative(primative_name); typedef struct primative_name { inherit_primative(super_primative);
#define define_primative_end(primative_name)                    } primative_name;

/* marcos for defining the creation and destruction of primatives (for .c file) */

#define define_primative_class_begin(primative_name, super_primative_name) primative_class PrimativeClassForPrimative(primative_name) = { .size = sizeof(struct primative_name), .super_primative = &PrimativeClassForPrimative(super_primative_name) 
#define primative_class_using_initialize(initialize_function) , .initialize = (init_fn)&initialize_function
#define primative_class_using_destroy(destroy_function)       , .destroy = (destroy_fn)&destroy_function
#define primative_class_using_copy(copy_function)             , .copy = (copy_fn)&copy_function
#define define_primative_class_end(primative_name) };

#define output *

define_primative_begin(autodisown_pool, primative_class)
	struct autodisown_pool *previous_pool;
	struct primative **autodisowned_objects;
define_primative_end(autodisown_pool)

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

primative *__make_instance(primative_class c);
primative *(*__make_initialize_fn(primative_class c))(primative *, ...);
#define __make(primative_class_name, ...) __make_initialize_fn(primative_class_name)(__make_instance(primative_class_name) , ## __VA_ARGS__)
#define make(primative_name, ...) (primative_name *)__make(PrimativeClassForPrimative(primative_name) , ## __VA_ARGS__)
extern void *copy(void *);
extern void destroy(void *);

#define SuperInitialize(p, ...) SuperPrimativeClass(p).initialize(p , ## __VA_ARGS__)
#define SuperCopy(p) SuperPrimativeClass(p).copy((primative *)p)
#define SuperDestroy(p) SuperPrimativeClass(p).destroy((primative *)p)

#endif