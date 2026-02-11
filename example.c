#include <stdio.h>
#include "my_alloc.h"

int main(void)
{
#ifdef DEBUG
    print_allocator();
#endif
    void *allocation = my_alloc(200);

    printf("%p \n", allocation);

#ifdef DEBUG
    print_allocator();
#endif
}