#include "my_alloc.h"
#include <sys/mman.h>

#ifdef DEBUG
#include <stdio.h>
#endif

typedef struct my_alloc_block
{
    size_t size;                 // payload size w/o header
    int free;                    // 1 = free, 0 = used
    struct my_alloc_block *next; // next free block
} my_alloc_block_t;

typedef struct
{
    void *base;
    my_alloc_block_t *first;
} my_alloc_t;

static my_alloc_t my_alloc_state = {.base = NULL, .first = NULL};

int my_alloc_init(my_alloc_t *alloc)
{
    void *mem = mmap(
        NULL,
        ARENA_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0);

    if (mem == MAP_FAILED)
        return -1;

    // create the first free block
    my_alloc_block_t *first = (my_alloc_block_t *)mem;
    first->size = ARENA_SIZE - sizeof(my_alloc_block_t);
    first->free = 1;
    first->next = NULL;

    // add block as first in allocator's free list
    alloc->first = first;
    alloc->base = mem;

    return 0;
}

// simple first fit allocator
static my_alloc_block_t *find_free_block(my_alloc_t *alloc, size_t size)
{
    my_alloc_block_t *prev = NULL;
    my_alloc_block_t *curr = alloc->first;

    while (curr)
    {
        if (curr->free && curr->size >= size)
        {
            if (curr == my_alloc_state.first)
                alloc->first = curr->next;
            else
                prev->next = curr->next;

            curr->free = 0;
            curr->next = NULL;

            return curr;
        }

        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

#ifdef DEBUG
static void _print_block(my_alloc_block_t *block)
{
    printf("addr=%p payload=%p size=%zu free=%d next=%p\n",
           (void *)block,
           (void *)(block + 1),
           block->size,
           block->free,
           (void *)block->next);
}

static void _print_allocator(my_alloc_t *alloc)
{
    printf("[my_alloc] allocator state:\n");

    if (alloc->base == NULL)
    {
        printf("  (not initialized)\n");
        return;
    }

    printf("    base=%p free_head=%p\n", alloc->base, (void *)alloc->first);

    printf("\n");

    printf("[my_alloc] arena layout:\n");

    my_alloc_block_t *curr = (my_alloc_block_t *)alloc->base;
    char *arena_end = (char *)alloc->base + ARENA_SIZE;

    while ((char *)curr < arena_end)
    {
        printf("    ");
        _print_block(curr);

        curr = (my_alloc_block_t *)((char *)(curr + 1) + curr->size);
    }
}

void print_allocator()
{
    _print_allocator(&my_alloc_state);
}
#endif

void *my_alloc(size_t size)
{
    if (size == 0)
        return NULL;

    if (my_alloc_state.first == NULL && my_alloc_init(&my_alloc_state) < 0)
        return NULL;

    my_alloc_block_t *block = find_free_block(&my_alloc_state, size);
    block->free = 0;

    return (block == NULL) ? NULL : (void *)(block + 1);
}

void my_free(void *ptr)
{
}