#include "common.h"
#include "unit.h"

// #define TEST_DEBUG_PRINT
#ifdef TEST_DEBUG_PRINT
static void print_int(void *a)
{
    printf("%d ", *(int *) a);
}

static void print_int_heap(struct heap_s *h)
{
    heap_array_do(h, print_int);
    printf("\n");
}
#else
#define print_int_heap(h)
#define print_int(a)
#endif

unit_test(heap_new)
{
    struct heap_s *h = heap_new(0);
    pass(new, h, "couldn't create new heap");
    heap_free(h);
}

unit_test(heap_insert)
{
    int value = 10;
    struct heap_s *h = heap_new(0);

    pass(new, h, "couldn't create new heap");
    pass(insert, heap_insert(h, &value), "couldn't insert %d", value);
    heap_free(h);
}

unit_test(heap_remove)
{
    struct heap_s *h = heap_new(0);
    int values[] = {10, 2, 5};
    size_t i;

    for(i = 0; i < 3; i++) {
        pass(insert, heap_insert(h, values + i),
            "couldn't insert %d at %lu", values[i], i);

        pass(num, h->num == (i + 1),
            "should be %lu, is: %lu", i + 1, h->num);
    }

    for(i = 0; i < 3; i++) {
        pass(remove, heap_remove(h), "couldn't remove item %lu", i);

        pass(num, h->num == (3 - i - 1),
            "should be %lu, is: %lu", 3 - i - 1, h->num);
    }

    heap_free(h);
}

unit_test(heap_sort)
{
    struct heap_s *h = heap_new(0);
    int values[] = {3, 10, 2, 5, 12}, sorted_indecies[] = { 2, 0, 3, 1, 4 },
        num_values = sizeof(values) / sizeof(int);
    size_t i;

    for(i = 0; i < num_values; i++) {
        pass(insert, heap_insert(h, values + i),
            "couldn't insert %d at %lu", values[i], i);

        pass(num, h->num == (i + 1),
            "should be %lu, is: %lu", i + 1, h->num);
    }

    for(i = 0; i < num_values; i++) {
        int val;
        pass(remove,
            (val = *(int *) heap_remove(h)) == values[sorted_indecies[i]],
            "should be %d, is %d", values[sorted_indecies[i]], val);

        pass(num, h->num == (num_values - i - 1),
            "should be %lu, is: %lu", num_values - i - 1, h->num);
    }

    heap_free(h);
}

unit_test(heapify)
{
    size_t i;
    int values[] = {3, 10, 2, 5, 12}, sorted_indecies[] = { 2, 0, 3, 1, 4 },
        num_values = sizeof(values) / sizeof(int);

    struct heap_s *h = heapify(values, sizeof(int), num_values, 0);

    for(i = 0; i < num_values; i++) {
        int val;
        print_int_heap(h);
        pass(remove,
            (val = *(int *) heap_remove(h)) == values[sorted_indecies[i]],
            "should be %d, is %d", values[sorted_indecies[i]], val);

        pass(num, h->num == (num_values - i - 1),
            "should be %lu, is: %lu", num_values - i - 1, h->num);
    }

    heap_free(h);
}

unit_test(unit)
{
    pass(unit, true, "must pass");
}

int main(int argc, char const *argv[])
{
    test(unit);
    test(heap_new);
    test(heap_insert);
    test(heap_remove);
    test(heap_sort);
    test(heapify);
    return 0;
}
