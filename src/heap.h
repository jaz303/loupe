#ifndef __HEAP_H__
#define __HEAP_H__

/*
 * heap implementation
 * TODO: shrink arrays when utilization drops < 50%
 * TODO: implement max capacity
 */

/* 
 * prototype of callback function for finding a heap item's priority.
 * it's called frequently so make it efficient
 */
typedef int (*heap_priority_f)(void*);

enum {
    MIN_HEAP    = 1,
    MAX_HEAP    = 2
};

typedef struct heap_t {
    void                **heap;
    int                 type;
    heap_priority_f     priority;
    int                 n_items;
    int                 c_items;
} heap_t;

heap_t *min_heap_new(heap_priority_f priority);
int min_heap_init(heap_t *heap, heap_priority_f priority);

heap_t *max_heap_new(heap_priority_f priority);
int max_heap_init(heap_t *heap, heap_priority_f priority);

heap_t *heap_new(int type, heap_priority_f priority);
int heap_init(heap_t *heap, int type, heap_priority_f priority);
void heap_delete(heap_t *heap);

int heap_size(heap_t *heap);
int heap_insert(heap_t *heap, void *item);
void *heap_take(heap_t *heap);

#endif