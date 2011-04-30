#include "heap.h"

#include <stdlib.h>

#define HEAP_INITIAL_SIZE   32
#define HEAP_PARENT(ix)     ((ix-1)/2)
#define HEAP_LEFT_IX(ix)    ((2*ix)+1)
#define HEAP_RIGHT_IX(ix)   ((2*ix)+2)
#define HEAP_P(h, ix)       (h->priority(h->heap[ix]))
#define HEAP_SIZE(h)        (h->c_items)
#define HEAP_ROOT(h)        (h->heap[0])

heap_t *min_heap_new(heap_priority_f priority) {
    return heap_new(MIN_HEAP, priority);
}

int min_heap_init(heap_t *heap, heap_priority_f priority) {
    return heap_init(heap, MIN_HEAP, priority);
}

heap_t *max_heap_new(heap_priority_f priority) {
    return heap_new(MAX_HEAP, priority);
}

int max_heap_init(heap_t *heap, heap_priority_f priority) {
    return heap_init(heap, MAX_HEAP, priority);
}

heap_t *heap_new(int type, heap_priority_f priority) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    if (!heap_init(heap, type, priority)) {
        free(heap);
        return NULL;
    }
    return heap;
}

int heap_init(heap_t *heap, int type, heap_priority_f priority) {
    heap->heap = malloc(sizeof(void*) * HEAP_INITIAL_SIZE);
    heap->type = type;
    heap->priority = priority;
    if (!heap->heap) {
        return 0;
    }
    heap->n_items = HEAP_INITIAL_SIZE;
    heap->c_items = 0;
    return 1;
}

int heap_size(heap_t *heap) {
    return heap->c_items;
}

int heap_insert(heap_t *heap, void *item) {
    
    /* grow heap if necessary */
    if (heap->c_items == heap->n_items) {
        heap->n_items *= 2;
        heap->heap = realloc(heap->heap, sizeof(void*) * heap->n_items);
        if (!heap->heap) {
            heap->n_items /= 2;
            return 0;
        }
    }
    
    int priority = heap->priority(item),
        ix = heap->c_items++;
    
    /* store item at bottom of tree */
    heap->heap[ix] = item;
    
    /* bubble-up */
    int parent = HEAP_PARENT(ix);
    
    if (heap->type == MIN_HEAP) {
        while ((ix > 0) && heap->priority(heap->heap[parent]) > priority) {
            void *tmp = heap->heap[parent];
            heap->heap[parent] = heap->heap[ix];
            heap->heap[ix] = tmp;
            ix = parent;
            parent = HEAP_PARENT(ix);
        }
    } else {
        while ((ix > 0) && heap->priority(heap->heap[parent]) < priority) {
            void *tmp = heap->heap[parent];
            heap->heap[parent] = heap->heap[ix];
            heap->heap[ix] = tmp;
            ix = parent;
            parent = HEAP_PARENT(ix);
        }
    }
    
    return 1;

}

void *heap_take(heap_t *h) {
    
    if (HEAP_SIZE(h) == 0) {
        return NULL;
    }
    
    void *out = HEAP_ROOT(h);
    h->heap[0] = h->heap[--h->c_items];
    
    int ix = 0;
    
    if (h->type == MIN_HEAP) {
        while (1) {
            int left_ix     = HEAP_LEFT_IX(ix),
                right_ix    = HEAP_RIGHT_IX(ix),
                smallest_ix = ix;

            if (left_ix < HEAP_SIZE(h) && HEAP_P(h, left_ix) < HEAP_P(h, ix)) {
                smallest_ix = left_ix;
            }

            if (right_ix < HEAP_SIZE(h) && HEAP_P(h, right_ix) < HEAP_P(h, smallest_ix)) {
                smallest_ix = right_ix;
            }

            if (smallest_ix != ix) {
                void *tmp = h->heap[ix];
                h->heap[ix] = h->heap[smallest_ix];
                h->heap[smallest_ix] = tmp;
                ix = smallest_ix;
            } else {
                break;
            }
        }
    } else {
        while (1) {
            int left_ix     = HEAP_LEFT_IX(ix),
                right_ix    = HEAP_RIGHT_IX(ix),
                largest_ix  = ix;

            if (left_ix < HEAP_SIZE(h) && HEAP_P(h, left_ix) > HEAP_P(h, ix)) {
                largest_ix = left_ix;
            }

            if (right_ix < HEAP_SIZE(h) && HEAP_P(h, right_ix) > HEAP_P(h, largest_ix)) {
                largest_ix = right_ix;
            }

            if (largest_ix != ix) {
                void *tmp = h->heap[ix];
                h->heap[ix] = h->heap[largest_ix];
                h->heap[largest_ix] = tmp;
                ix = largest_ix;
            } else {
                break;
            }
        }
    }
    
    return out;

}