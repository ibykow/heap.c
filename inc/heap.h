#ifndef HEAP_H_
#define HEAP_H_

#define HEAP_DEFAULT_DIFF   0

typedef int (*heap_diff_fn)(void *a, void *b);

struct heap_s {
    size_t num, len;
    heap_diff_fn diff;
    void **array;
};

struct heap_s *heap_new(heap_diff_fn);
struct heap_s *heapify(void *, size_t, size_t, heap_diff_fn);
void heap_free(struct heap_s *);

size_t heap_insert(struct heap_s *, void *);
size_t heap_insert_all(struct heap_s *, void *, size_t, size_t);
void *heap_remove(struct heap_s *);
void heap_array_do(struct heap_s *, void (*)(void *));

#endif
