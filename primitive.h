#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

struct primitive_class;
struct primitive;

typedef struct primitive       primitive;           // primitive
typedef struct primitive_class primitive_class;     // primitive_class

typedef primitive *(*init_fn)(primitive *, ...);	// init_fn
typedef void (*destroy_fn)(primitive *);          	// destroy_fn
typedef primitive *(*copy_fn)(primitive *);         // copy_fn

struct primitive_class {
	size_t      size;
	init_fn     initialize;
	destroy_fn  destroy;
	copy_fn     copy;
	size_t      ownership_count;
	struct primitive_class *super_primitive;
};

struct primitive {
	primitive_class primitive;
};

extern primitive_class primitive_PrimitiveClass;

#define PrimitiveClass(p) (*(primitive_class *)(p))
#define PrimitiveClassForPrimitive(p) p ## _PrimitiveClass
#define inherit_primitive(super_primitive) super_primitive primitive
#define SuperPrimitiveClass(p) (*(PrimitiveClass(p).super_primitive))

/* macros for declaring the existance and structure of primitives (for .h file) */

#define primitive_define(primitive_name, super_primitive, structure) \
	extern primitive_class PrimitiveClassForPrimitive(primitive_name); \
	typedef struct primitive_name { inherit_primitive(super_primitive); struct structure; } primitive_name;

// #define define_primitive_begin(primitive_name, super_primitive) extern primitive_class PrimitiveClassForPrimitive(primitive_name); typedef struct primitive_name { inherit_primitive(super_primitive);
// #define define_primitive_end(primitive_name)                    } primitive_name;

/* marcos for defining the creation and destruction of primitives (for .c file) */

#define primitive_class_define(primitive_name, super_primitive_name, ...) \
	primitive_class PrimitiveClassForPrimitive(primitive_name) = { \
		.size = sizeof(struct primitive_name), \
		.super_primitive = &PrimitiveClassForPrimitive(super_primitive_name), ## __VA_ARGS__ };
#define using_initialize(fn)	.initialize = (init_fn)&fn
#define using_destroy(fn) 		.destroy = (destroy_fn)&fn
#define using_copy(fn)			.copy = (copy_fn)&fn

#define output *

primitive_define(autodisown_pool, primitive_class, {
	struct autodisown_pool *previous_pool;
	struct primitive **autodisowned_objects;
});

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

primitive *__make_instance(primitive_class c);
primitive *(*__make_initialize_fn(primitive_class c))(primitive *, ...);
#define __make(primitive_class_name, ...) __make_initialize_fn(primitive_class_name)(__make_instance(primitive_class_name) , ## __VA_ARGS__)
#define make(primitive_name, ...) (primitive_name *)__make(PrimitiveClassForPrimitive(primitive_name) , ## __VA_ARGS__)
extern void *copy(void *);
extern void destroy(void *);

#define SuperInitialize(p, ...) SuperPrimitiveClass(p).initialize(p , ## __VA_ARGS__)
#define SuperCopy(p) SuperPrimitiveClass(p).copy((primitive *)p)
#define SuperDestroy(p) SuperPrimitiveClass(p).destroy((primitive *)p)

#endif