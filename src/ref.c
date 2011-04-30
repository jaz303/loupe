#include "ref.h"

ref_allocator_t ref_allocator_default = { malloc, realloc, free };

void *ref_alloc(ref_allocator_t *allocator, size_t sz) {
    allocator = FIND_ALLOCATOR(allocator);
    void *ref = allocator->alloc(sz);
    if (ref != NULL) {
        ((ref_t*)ref)->retain_count = 1;
        ((ref_t*)ref)->allocator = allocator;
    }
    return ref;
}

void ref_retain(void *ref) {
    ((ref_t*)ref)->retain_count++;
}

void ref_release(void *ref) {
    if ((--((ref_t*)ref)->retain_count) == 0) {
        ((ref_t*)ref)->allocator->free(ref);
    }
}
