#include "common.h"

#define HEAP_PI(ci) (ci ? (ci - 1) >> 1 : ci)
#define HEAP_LCI(i) ((i << 1) + 1)
#define HEAP_RCI(i) (HEAP_LCI(i) + 1)

static int int_diff(void *a, void *b)
{
    return *(int *) a - *(int *) b;
}

static bool heap_resize(struct heap_s *h, size_t len)
{
    void **arr = (void **) realloc(h->array, len * sizeof(void *));

    if(len && !arr)
        return false;

    h->len = len;
    h->array = arr;

    return true;
}

struct heap_s *heap_new(heap_diff_fn diff)
{
    struct heap_s *h = (struct heap_s *) malloc(sizeof(struct heap_s));
    if(!h)
        return 0;

    h->array = 0,
    h->num = h->len = 0,
    h->diff = diff ? diff : int_diff;

    return h;
}

struct heap_s *heapify(void *data, size_t size, size_t len, heap_diff_fn diff)
{
    if(!data || !size || !len)
        return 0;

    struct heap_s *h = heap_new(diff);
    if(!h)
        return 0;

    heap_insert_all(h, data, size, len);

    return h;
}

void heap_free(struct heap_s *h)
{
    if(!h)
        return;

    if(h->len)
        free(h->array);

    free(h);
}

size_t heap_insert(struct heap_s *h, void *v)
{
    if(!h || !v)
        return 0;

    /* Resize the array if necessary */
    if((h->num <= h->len) && (!heap_resize(h, (h->len << 1) + 1)))
        return 0;

    size_t pi, i;

    /* insert up */
    for(i = h->num; i; i = pi) {
        pi = HEAP_PI(i);

        int diff = h->diff(v, h->array[pi]);
        if(diff >= 0)
            /* point to v here */
            break;

        /* move the parent pointer down */
        h->array[i] = h->array[pi];
    }

    h->array[i] = v;

    return ++h->num;
}

size_t heap_insert_all(struct heap_s *h, void *data, size_t size, size_t len)
{
    if(!h || !data)
        return 0;

    if(((h->num + len) <= h->len) && !heap_resize(h, h->len + (len << 1)))
        return 0;

    size_t i, max = h->num + len;
    for(i = h->num; i < max; i++)
        heap_insert(h, (char *) data + (i * size));

    return h->num;
}

void *heap_remove(struct heap_s *h)
{
    if(!h || !h->num)
        return 0;

    void *ret = h->array[0];

    size_t pi, lci, rci, nci, last_index = h->num - 1;

    for(nci = pi = 0, lci = 1, rci = 2; lci < h->num;
            pi = nci, lci = HEAP_LCI(nci), rci = lci + 1) {

        /* chose the left or right child, whichever is smaller */
        nci = (rci < h->num) &&
            (h->diff(h->array[lci], h->array[rci]) > 0)
            ? rci : lci;

        /* check if we should stop here */
        if(h->diff(h->array[last_index], h->array[nci]) <= 0)
            break;

        /* otherwise, keep moving the pointers up */
        h->array[pi] = h->array[nci];
    }

    h->array[pi] = h->array[last_index];
    h->num--;

    /*
     * shrink the h->array by half if the number of entries in h (h->num)
     * is less than a quarter of the allocated length of the array (h->len)
     */
    if(h->num < (h->len >> 2))
        heap_resize(h, h->len >> 1);

    return ret;
}

void heap_array_do(struct heap_s *h, void (*iter)(void*))
{
    if(!h)
        return;

    size_t i;
    for(i = 0; i < h->num; i++)
        iter(h->array[i]);
}
