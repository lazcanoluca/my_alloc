#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <stddef.h>

#define ARENA_SIZE (1 << 20) // 1 MiB

void *my_alloc(size_t size);
void my_free(void *ptr);

#ifdef DEBUG
void print_allocator();
#endif

#endif