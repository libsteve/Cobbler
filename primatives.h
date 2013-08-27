#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>

struct primative_class;
struct primative;

typedef struct primative       primative;            // primative
typedef struct primative_class primative_class;      // primative_class

typedef primative *(*init_fn)(primative *, va_list *args_p);  // init_fn
typedef void (*destroy_fn)(primative *);                      // destroy_fn
typedef primative *(*copy_fn)(primative *);                   // copy_fn

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

extern primative_class PrimativeClass;

#define PrimativeClass(p) (*(primative_class *)(p))
#define inherit_primative(super_primative) super_primative primative

/* macros for declaring the existance and structure of primatives (for .h file) */

#define declare_primative_begin(primative_name, primative_class_name, super_primative) extern primative_class primative_class_name; typedef struct primative_name { inherit_primative(super_primative);
#define declare_primative_end(primative_name)                    } primative_name;

/* marcos for defining the creation and destruction of primatives (for .c file) */

#define define_primative_class_begin(primative_name, primative_class_name, super_primative_class_name) primative_class primative_class_name = { .size = sizeof(struct primative_name), .super_primative = &super_primative_class_name
#define primative_class_using_initialize(initialize_function) , .initialize = (init_fn)&initialize_function
#define primative_class_using_destroy(destroy_function)       , .destroy = (destroy_fn)&destroy_function
#define primative_class_using_copy(copy_function)             , .copy = (copy_fn)&copy_function
#define define_primative_class_end(primative_name) };

#define output *

declare_primative_begin(autodisown_pool, autodisown_poolPrimative, primative_class)
	struct autodisown_pool *previous_pool;
	struct primative **autodisowned_objects;
declare_primative_end(autodisown_pool)

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

extern void *make(primative_class, ...);
extern void *copy(void *);
extern void destroy(void *);

extern primative_class PrimativeNull;

#endif