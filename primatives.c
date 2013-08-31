#include "primatives.h"
#include <stdio.h>

// this is to compensate for clang's lack of C11's <threads.h> header :/
#include <pthread.h>
#define tss_create(key, fn) pthread_key_create(key, fn)
#define tss_get(key) pthread_getspecific(key)
#define tss_set(key, value) pthread_setspecific(key, value)
#define thrd_success 0
#define tss_t pthread_key_t

/////////////////////////////////////////
// Primative Definitions

primative *_default_initialize(primative *p) {
	return p;
}

primative *_default_copy(primative *p) {
	return NULL;
}

void _default_destroy(primative *p) {
	return;
}

define_primative_class_begin(primative, primative)
	primative_class_using_initialize(_default_initialize)
	primative_class_using_copy(_default_copy)
	primative_class_using_destroy(_default_destroy)   
define_primative_class_end(primative)

/////////////////////////////////////////
// Autodisown Definitions

static tss_t autodisown_pool_tss_id;

void autodisown_tss_dtor(void *v) {
	autodisown_pool *p = v;
	while (p != NULL) {
		autodisown_pool *next = p->previous_pool;
		disown(p);
		p = next;
	}
}

void __set_autodisown_pool(autodisown_pool *pool) {
	int result = tss_set(autodisown_pool_tss_id, pool);
	if (result != thrd_success) {
		fprintf(stderr, "%s\n", "Problem setting autodisown_pool to thread specific storage");
		exit(-1);
	}
}

autodisown_pool *__autodisown_pool() {
	return tss_get(autodisown_pool_tss_id);
}

autodisown_pool *autodisown_pool_initialize(autodisown_pool *pool, va_list *args) {
	if (pool) {
		static bool thread_specific_storage_is_initialized = false;
		if (!thread_specific_storage_is_initialized) {
			thread_specific_storage_is_initialized = true;
			int result = tss_create(&autodisown_pool_tss_id, &autodisown_tss_dtor);
			if (result != thrd_success) {
				fprintf(stderr, "%s\n", "Problem allocating thread specific storage for autodisown_pool");
				exit(-1);
			}
		}
		autodisown_pool *previous = __autodisown_pool();
		pool->previous_pool = own(previous);
		__set_autodisown_pool(pool);
	}
	return pool;
}

void autodisown_pool_destroy(autodisown_pool *pool) {
	if (pool) {
		if (pool->autodisowned_objects) {
			uint32_t count = 0;
			for (primative *p = pool->autodisowned_objects[count]; 
				 p != NULL; 
				 p = pool->autodisowned_objects[++count]) {
				disown(p);
			}
		}
		__set_autodisown_pool(pool->previous_pool);
		disown(pool->previous_pool);
	}
	pool->primative.destroy((primative *)pool);
}

define_primative_class_begin(autodisown_pool, primative)
	primative_class_using_initialize(autodisown_pool_initialize)
	primative_class_using_destroy(autodisown_pool_destroy)
define_primative_class_end(autodisown_pool)

void *own(void *v) {
	primative *p = v;
	if (p != NULL) {
		p->primative.ownership_count += 1;
	}
	return p;
}

void *disown(void *v) {
	primative *p = v;
	if (p != NULL) {
		if (PrimativeClass(p).ownership_count == 0) {
			return p;
		}
		PrimativeClass(p).ownership_count -= 1;
		if (PrimativeClass(p).ownership_count == 0) {
			destroy(p);
		}
	}
	return p;
}

void *autodisown(void *v) {
	primative *p = v;
	autodisown_pool *pool = __autodisown_pool();
	if (pool) {
		int count = 0;
		for (int i=0; pool->autodisowned_objects[i] != NULL; i++, count++) {
			primative *autodisowned = pool->autodisowned_objects[i];
			if (autodisowned == p)
				return p;
		}
		pool->autodisowned_objects = realloc(pool->autodisowned_objects, (count + 2) * sizeof(primative *));
		pool->autodisowned_objects[count] = p;
		pool->autodisowned_objects[count+1] = NULL;
	}
	return p;
}

/////////////////////////////////////////
// Creation Definitions

primative *__make_instance(primative_class c) {
	primative *p = calloc(1, c.size);
	p->primative = c;
	return own(p);
}

primative *__make_initialize_none(primative *p, ...) {
	return p;
}

primative *(*__make_initialize_fn(primative_class c))(primative *, ...) {
	if (c.initialize != NULL) {
		return c.initialize;
	} else if (c.super_primative != NULL) {
		return __make_initialize_fn(*(c.super_primative));
	}
	return __make_initialize_none;
}

void *__copy(primative *p, primative_class c) {
	if (c.copy != NULL) {
		return c.copy(p);
	} else if (c.super_primative != NULL) {
		return __copy(p, *(c.super_primative));
	}
	return NULL;
}

void *copy(void *v) {
	primative *p = v;
	primative *copy = NULL;
	if (p != NULL) {
		copy = __copy(p, PrimativeClass(p));
	}
	return copy;
}

void __destroy(primative *p, primative_class c) {
	if (c.destroy != NULL) {
		c.destroy(p);
	} else if (c.super_primative != NULL) {
		__destroy(p, *(c.super_primative));
	}
}

void destroy(void *v) {
	primative *p = v;
	if (p != NULL) {
		__destroy(p, PrimativeClass(p));
	}
	free(p);
}
