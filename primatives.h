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

#define output *

typedef struct autodisown_pool {
	inherit_primative(primative_class);
	struct autodisown_pool *previous_pool;
	struct primative **autodisowned_objects;
} autodisown_pool;
extern primative_class autodisown_poolPrimative;

extern void *own(void *);
extern void *disown(void *);
extern void *autodisown(void *);

extern void *make(primative_class, ...);
extern void *copy(void *);
extern void destroy(void *);

extern primative_class PrimativeNull;

#endif