#include "primitive.h"
#include <stdio.h>

//////
// primitive instance creation implementations

primitive *__create_instance(primitive_class *c) {
    primitive *p = calloc(1, c->size);
    p->primitive = *c;
    return own(p);
}

void *copy(void *v) {
    primitive *p = v;
    primitive *copy = NULL;
    if (p != NULL) {
        copy = virtual_call(primitive *, copy, p);
    }
    return copy;
}

void destroy(void *v) {
    primitive *p = v;
    if (p != NULL) {
        virtual_call(void, destroy, p);
    }
    free(p);
}

//////
// default primitive virtual method implementations

primitive *method(primitive, create)  { return this; }
primitive *method(primitive, copy)    { return NULL; }
void       method(primitive, destroy) { return;      }

//////
// reference counting implementations

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
        if (PrimitiveCast(primitive_class, p)->ownership_count == 0) {
            printf("cannot disown without ownership count\n");
            return p;
        }
        PrimitiveCast(primitive_class, p)->ownership_count -= 1;
        if (PrimitiveCast(primitive_class, p)->ownership_count == 0) {
            destroy(p);
            return NULL;
        }
    }
    return p;
}

//////
// autodisown implementation

// this is to compensate for clang's lack of C11's <threads.h> header :/
#include <pthread.h>
#define tss_create(key, fn) pthread_key_create(key, fn)
#define tss_get(key) pthread_getspecific(key)
#define tss_set(key, value) pthread_setspecific(key, value)
#define thrd_success 0
#define tss_t pthread_key_t

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

autodisown_pool *
method(autodisown_pool, create) {
    autodisown_pool *pool = this;
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

void
method(autodisown_pool, destroy) {
    autodisown_pool *pool = this;
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
    SuperDestroy();
}

void *autodisown(void *v) {
    return v;
    // TODO: fix the autodisown feature
    primitive *p = v;
    autodisown_pool *pool = __autodisown_pool();
    printf("attempting autodisown\n");
    if (pool) {
        printf("autodisowning\n");
        int count = 0;
        for (; pool->autodisowned_objects[count] != NULL; count++) {
            primitive *autodisowned = pool->autodisowned_objects[count];
            if (autodisowned == p)
                return p;
        }
        pool->autodisowned_objects = realloc(pool->autodisowned_objects, (count + 2) * sizeof(primitive *));
        pool->autodisowned_objects[count] = p;
        pool->autodisowned_objects[count+1] = NULL;
    }
    return p;
}
