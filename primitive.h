#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct primitive;
struct primitive_class;
struct virtual_method;

typedef struct primitive       primitive;           // primitive
typedef struct primitive_class primitive_class;     // primitive_class
typedef struct virtual_method  virtual_method;

typedef primitive *(*init_fn)(primitive *, ...);	// init_fn
typedef void (*destroy_fn)(primitive *);          	// destroy_fn
typedef primitive *(*copy_fn)(primitive *);         // copy_fn

struct primitive_class {
	size_t      size;
	const char  *classname;
	size_t      method_count;
	size_t      ownership_count;
	struct virtual_method   *methods;
	struct primitive_class  *super_primitive;
};

#define PrimitiveClass(p) (*(primitive_class *)(p))
#define PrimitiveClassForPrimitive(p) p ## _PrimitiveClass ()
#define inherit_primitive(super_primitive) super_primitive primitive
#define SuperPrimitiveClass(p) (PrimitiveClass(p).super_primitive)
#define PrimitiveClassName(p) #p

struct virtual_method {
	const char *signature;
	void *functionpointer;
};

static inline void *virtual_method_lookup(primitive_class *c, const char *fn) {
	for (int i = 0; i < c->method_count; i++) {
		if (strcmp(c->methods[i].signature, fn) == 0) {
			return c->methods[i].functionpointer;
		}
	}
	if (c->super_primitive != NULL) {
		return virtual_method_lookup(c->super_primitive, fn);
	}
	return (void *)0;
}

#define method_name(primitive_name, fn) primitive_name ## _ ## fn
#define method(primitive_name, fn, ...) method_name(primitive_name, fn) (primitive_name *self, ## __VA_ARGS__)
#define virtual_call(returns, instance, fn, ...) ((returns (*)(void *, ...)) virtual_method_lookup((primitive_class *)instance, #fn)) (instance, ## __VA_ARGS__)
#define static_call(primitive_name, instance, fn, ...) method_name(primitive_name, fn) ((primitive_name *)instance, ## __VA_ARGS__)
#define using_virtual(primitive_name, fn) (struct virtual_method){ .signature = #fn , .functionpointer = (void *)& method_name(primitive_name, fn) }

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
		static virtual_method methods[] = { __VA_ARGS__ }; \
		if (setup) { \
			setup = false; \
			c = (primitive_class){ \
				.size = sizeof(struct primitive_name), \
				.classname = PrimitiveClassName(primitive_name), \
				.method_count = sizeof(methods) / sizeof(struct virtual_method), \
				.methods = methods, \
				.super_primitive = PrimitiveClassForPrimitive(super_primitive_name)}; \
		} \
		return &c; \
	}

struct primitive {
	primitive_class primitive;
};

static inline primitive *method(primitive, initialize) { return self; }
static inline primitive *method(primitive, copy) { return NULL; }
static inline void method(primitive, destroy) { return; }

static inline primitive_class * PrimitiveClassForPrimitive(primitive) {
	static bool setup = true;
	static primitive_class c;
	static virtual_method methods[] = {
		using_virtual(primitive, initialize),
		using_virtual(primitive, copy),
		using_virtual(primitive, destroy)
	};
	if (setup) {
		setup = false;
		c = (primitive_class){
			.size = sizeof(struct primitive),
			.classname = PrimitiveClassName(primitive),
			.method_count = sizeof(methods) / sizeof(struct virtual_method),
			.methods = methods,
			.super_primitive = NULL};
	}
	return &c;
}

/* marcos for defining the creation and destruction of primitives (for .c file) */

#define output *

primitive_declare(autodisown_pool);
autodisown_pool *method(autodisown_pool, initialize, va_list *args);
void method(autodisown_pool, destroy);
primitive_define(autodisown_pool, primitive, {
		struct autodisown_pool *previous_pool;
		struct primitive **autodisowned_objects;
	},
	using_virtual(autodisown_pool, initialize), 
	using_virtual(autodisown_pool, destroy));

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

primitive *__make_instance(primitive_class *c);
#define __make(primitive_class_struct, ...)  virtual_call(void *, __make_instance(primitive_class_struct), initialize, ## __VA_ARGS__)
#define make(primitive_name, ...) (primitive_name *)__make(PrimitiveClassForPrimitive(primitive_name) , ## __VA_ARGS__)
extern void *copy(void *);
extern void destroy(void *);

#define SuperInitialize(p, ...) virtual_call(primitive *, SuperPrimitiveClass(p), initialize, ## __VA_ARGS__)
#define SuperCopy(p) virtual_call(primitive *, SuperPrimitiveClass(p), copy)
#define SuperDestroy(p) virtual_call(void, SuperPrimitiveClass(p), destroy)

#endif