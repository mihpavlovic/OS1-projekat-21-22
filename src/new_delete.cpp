//
// Created by os on 8/17/22.
//

#include "../h/syscall_c.h"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}


void operator delete[](void *p) noexcept
{
    mem_free(p);
}