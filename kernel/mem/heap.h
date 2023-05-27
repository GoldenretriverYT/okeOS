// Based on my old code so probably has some bugs:
// https://github.com/GoldenretriverYT/spagOS/blob/main/kernel/src/memory/heap.h
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "pageframeallocator.h"
#include "paging.h"

#define HEAPSEGHDR_MAGIC 0xDEADBEEF

struct HeapSegHdr{
    u32 magic = HEAPSEGHDR_MAGIC; // for debugging
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;

    bool free;
    void combineForward();
    void combineBackward();
    HeapSegHdr* split(size_t splitLength);
}__attribute__((packed));

void initHeap(void* heapAddress, size_t pageCount);
void expandHeap(size_t length);

void* malloc(size_t size);
void free(void* address);

/// @brief Secure free, fills the memory with 0s after freeing it
/// @param address 
/// @param size 
void sfree(void* address, size_t size);

size_t getUsedMem();


inline void* operator new(size_t size) noexcept {return malloc(size);}
inline void* operator new[](size_t size) noexcept {return malloc(size);}
inline void operator delete(void* p) noexcept {free(p);}
inline void operator delete[](void* p) noexcept {free(p);}
inline void operator delete(void* p, unsigned long s) noexcept {free(p);}