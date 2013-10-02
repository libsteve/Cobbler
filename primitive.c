#include "primitive.h"
#include <stdio.h>

// this is to compensate for clang's lack of C11's <threads.h> header :/
#include <pthread.h>
#define tss_create(key, fn) pthread_key_create(key, fn)
#define tss_get(key) pthread_getspecific(key)
#define tss_set(key, value) pthread_setspecific(key, value)
#define thrd_success 0
#define tss_t pthread_key_t

/////////////////////////////////////////
// primitive Definitions

primitive *_default_initialize(primitive *p) {
	return p;
}

primitive *_default_copy(primitive *p) {
	return NULL;
}

void _default_destroy(primitive *p) {
	return;
}

primitive_class_define(primitive, primitive,
	using_initialize(_default_initialize),
	using_copy(_default_copy),
	using_destroy(_default_destroy));

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
			for (primitive *p = pool->autodisowned_objects[count]; 
				 p != NULL; 
				 p = pool->autodisowned_objects[++count]) {
				disown(p);
			}
		}
		__set_autodisown_pool(pool->previous_pool);
		disown(pool->previous_pool);
	}
	pool->primitive.destroy((primitive *)pool);
}

primitive_class_define(autodisown_pool, primitive, 
	using_initialize(autodisown_pool_initialize),
	using_destroy(autodisown_pool_destroy));

void *own(void *v) {
	primitive *p = v;
	if (p != NULL) {
		p->primitive.ownership_count += 1;
	}
	return p;
}

void *disown(void *v) {
	primitive *p = v;
	if (p != NULL) {
		if (PrimitiveClass(p).ownership_count == 0) {
			return p;
		}
		PrimitiveClass(p).ownership_count -= 1;
		if (PrimitiveClass(p).ownership_count == 0) {
			destroy(p);
		}
	}
	return p;
}

void *autodisown(void *v) {
	primitive *p = v;
	autodisown_pool *pool = __autodisown_pool();
	if (pool) {
		int count = 0;
		for (int i=0; pool->autodisowned_objects[i] != NULL; i++, count++) {
			primitive *autodisowned = pool->autodisowned_objects[i];
			if (autodisowned == p)
				return p;
		}
		pool->autodisowned_objects = realloc(pool->autodisowned_objects, (count + 2) * sizeof(primitive *));
		pool->autodisowned_objects[count] = p;
		pool->autodisowned_objects[count+1] = NULL;
	}
	return p;
}

/////////////////////////////////////////
// Creation Definitions

primitive *__make_instance(primitive_class c) {
	primitive *p = calloc(1, c.size);
	p->primitive = c;
	return own(p);
}

primitive *__make_initialize_none(primitive *p, ...) {
	return p;
}

primitive *(*__make_initialize_fn(primitive_class c))(primitive *, ...) {
	if (c.initialize != NULL) {
		return c.initialize;
	} else if (c.super_primitive != NULL) {
		return __make_initialize_fn(*(c.super_primitive));
	}
	return __make_initialize_none;
}

void *__copy(primitive *p, primitive_class c) {
	if (c.copy != NULL) {
		return c.copy(p);
	} else if (c.super_primitive != NULL) {
		return __copy(p, *(c.super_primitive));
	}
	return NULL;
}

void *copy(void *v) {
	primitive *p = v;
	primitive *copy = NULL;
	if (p != NULL) {
		copy = __copy(p, PrimitiveClass(p));
	}
	return copy;
}

void __destroy(primitive *p, primitive_class c) {
	if (c.destroy != NULL) {
		c.destroy(p);
	} else if (c.super_primitive != NULL) {
		__destroy(p, *(c.super_primitive));
	}
}

void destroy(void *v) {
	primitive *p = v;
	if (p != NULL) {
		__destroy(p, PrimitiveClass(p));
	}
	free(p);
}
