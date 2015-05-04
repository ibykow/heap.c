# heap.c
A binary heap implementation

This is a dumb heap, and I mean very dumb. It points to whatever data you pass it, and that's it.

It does not trust its own diff function, so repeat entries are gladly inserted the same as others.
That means it creates new pointers for repeat entries and wastes space.

Since the heap merely points to data located elsewhere, changing that data in a way which modifies its diff results means that the heap becomes corrupt and you have to re-heapify, by creating a new heap and passing the old heap's data to it one at a time. There is no convenience function for this operation at this time.

### Basics
Create an empty integer min heap:
```
struct heap_s *h = heap_new(0);
```
&nbsp;  
Insert an integer:
```
int n = 3;
heap_insert(h, &n);
```
&nbsp;  
Insert an array of integers into an existing heap:
```
int values[] = {3, 10, 2, 5, 12}, sorted_indecies[] = { 2, 0, 3, 1, 4 },
    num_values = sizeof(values) / sizeof(int);

heap_insert_all(h, values, sizeof(int), num_values);
```
&nbsp;  
Insert an array of integers into a new heap:
```
struct heap_s *h = heapify(values, sizeof(int), num_values, 0);
```
&nbsp;  
Remove an item:
```
int n = *(int *) heap_remove(h);
```
&nbsp;  
Free a heap:
```
heap_free(h);
```

### Custom data example

The following example creates a max-heap of pointers to a custom structure ```proc_info_s```.

The structure:
```
struct proc_info_s {
    const char *description;
    int id;
    unsigned priority;
};
```
&nbsp;  
The diff function:
```
/* b - a creates a max heap */
int proc_info_diff(void *a, void *b)
{
    return (int) ((struct proc_info_s *) b)->priority -
        (int) ((struct proc_info_s *) a)->priority;
}
```
&nbsp;  
The sample array:
```
#define new_proc_info(desc, id, priority) (struct proc_info_s) \
    { .description = desc, .id = id, .priority = priority }

struct process_info_s proc_infos[] = {
    new_process_info(idle, 1, 0),
    new_process_info(mouse tracker, 13, 9),
    new_process_info(network manager, 24, 6)
};

size_t  pinfo_size = sizeof(struct proc_info_s),
        num_pinfos = sizeof(proc_infos) / sizeof(struct proc_info_s);
```
&nbsp;  
Create the heap:
```
struct heap_s *h = heapify(proc_infos, pinfo_size, num_pinfos, proc_info_diff);
```
