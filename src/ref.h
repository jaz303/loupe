#ifndef __REF_H__
#define __REF_H__

#include <stdlib.h>

#define FIND_ALLOCATOR(a) (a == NULL ? &ref_allocator_default : a)

typedef void* (*ref_allocator_alloc_f)(size_t);
typedef void* (*ref_allocator_realloc_f)(void *, size_t);
typedef void (*ref_allocator_free_f)(void *);

typedef struct ref_allocator {
    ref_allocator_alloc_f       alloc;
    ref_allocator_realloc_f     realloc;
    ref_allocator_free_f        free;
} ref_allocator_t;

typedef struct ref {
    int                 retain_count;
    ref_allocator_t     *allocator;
} ref_t;

extern ref_allocator_t ref_allocator_default;

void *ref_alloc(ref_allocator_t *allocator, size_t sz);
void ref_retain(void *ref);
void ref_release(void *ref);

#endif