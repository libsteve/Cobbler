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

#define PrimitiveClass(p) (*(primitive_class *)(p))
#define PrimitiveClassForPrimitive(p) p ## _PrimitiveClass ()
#define inherit_primitive(super_primitive) super_primitive primitive
#define SuperPrimitiveClass(p) (*(PrimitiveClass(p).super_primitive))

/* macros for declaring the existance and structure of primitives (for .h file) */

#define primitive_declare(primitive_name) struct primitive_name; typedef struct primitive_name primitive_name;
#define primitive_define(primitive_name, super_primitive_name, structure, ...) \
	typedef struct primitive_name { \
		inherit_primitive(super_primitive_name); \
		struct structure; \
	} primitive_name; \
	static inline primitive_class * PrimitiveClassForPrimitive(primitive_name) { \
		static bool setup = true; \
		static primitive_class c; \
		if (setup) { \
			setup = false; \
			c = (primitive_class){ \
				.size = sizeof(struct primitive_name), \
				.super_primitive = PrimitiveClassForPrimitive(super_primitive_name), ## __VA_ARGS__ }; \
		} \
		return &c; \
	}
#define using_initialize(fn)	.initialize = (init_fn)&fn
#define using_destroy(fn) 		.destroy = (destroy_fn)&fn
#define using_copy(fn)			.copy = (copy_fn)&fn

struct primitive {
	primitive_class primitive;
};

static inline primitive *_default_initialize(primitive *p) { return p; }
static inline primitive *_default_copy(primitive *p) { return NULL; }
static inline void _default_destroy(primitive *p) { return; }

static inline primitive_class * PrimitiveClassForPrimitive(primitive) {
	static bool setup = true;
	static primitive_class c;
	if (setup) {
		setup = false;
		c = (primitive_class){
			.size = sizeof(struct primitive),
			.super_primitive = PrimitiveClassForPrimitive(primitive),
			using_initialize(_default_initialize),
			using_copy(_default_copy),
			using_destroy(_default_destroy)
		};
	}
	return &c;
}

/* marcos for defining the creation and destruction of primitives (for .c file) */

#define output *

primitive_declare(autodisown_pool);
autodisown_pool *autodisown_pool_initialize(autodisown_pool *pool, va_list *args);
void autodisown_pool_destroy(autodisown_pool *pool);
primitive_define(autodisown_pool, primitive, {
		struct autodisown_pool *previous_pool;
		struct primitive **autodisowned_objects;
	},
	using_initialize(autodisown_pool_initialize), 
	using_destroy(autodisown_pool_destroy));

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

primitive *__make_instance(primitive_class *c);
primitive *(*__make_initialize_fn(primitive_class *c))(primitive *, ...);
#define __make(primitive_class_name, ...) __make_initialize_fn(primitive_class_name)(__make_instance(primitive_class_name) , ## __VA_ARGS__)
#define make(primitive_name, ...) (primitive_name *)__make(PrimitiveClassForPrimitive(primitive_name) , ## __VA_ARGS__)
extern void *copy(void *);
extern void destroy(void *);

#define SuperInitialize(p, ...) SuperPrimitiveClass(p).initialize(p , ## __VA_ARGS__)
#define SuperCopy(p) SuperPrimitiveClass(p).copy((primitive *)p)
#define SuperDestroy(p) SuperPrimitiveClass(p).destroy((primitive *)p)

#endif