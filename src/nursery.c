#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "nursery.h"

/**
 * mmap a contiguous region and split it into two semispaces
 * following a copy-and-swap model, where either is active.
 */
void nurseryInit(Nursery* n) {
    void* mem = mmap(NULL, 2 * NURSERY_SIZE, 
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON, -1, 0
    );
    if (mem == MAP_FAILED) {
        fprintf(stderr, "Failed to allocate nursery\n");
        exit(1);
    }
    n->fromSpace = (uint8_t*)mem;
    n->toSpace   = (uint8_t*)mem + NURSERY_SIZE;
    n->ptr       = n->fromSpace;
    n->limit     = n->fromSpace + NURSERY_SIZE;
}

void* nurseryAlloc(Nursery* n, size_t size) {
    size = ALIGN_UP(size, 8);
    if (n->ptr + size > n->limit) return NULL;
    void* result = n->ptr;
    n->ptr += size;
    return result;
}

void nurseryFree(Nursery* n) {
    if (n->fromSpace != NULL) {
        // For self-reference: `munmap` returns mmap-ped memory to the OS
        // as opposed to `free` (which returns it to the malloc heap)
        munmap(n->fromSpace < n->toSpace ? n->fromSpace : n->toSpace,
               2 * NURSERY_SIZE);
    }
    n->fromSpace = NULL;
    n->toSpace = NULL;
    n->ptr = NULL;
    n->limit = NULL;
}