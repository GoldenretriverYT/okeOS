// Based on my old code so probably has some bugs:
// https://github.com/GoldenretriverYT/spagOS/blob/main/kernel/src/memory/heap.h
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "pageframeallocator.h"
#include "paging.h"

struct HeapSegHdr{
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool free;
    void combineForward();
    void combineBackward();
    HeapSegHdr* split(size_t splitLength);
};

void initHeap(void* heapAddress, size_t pageCount);
void expandHeap(size_t length);

void* malloc(size_t size);
void free(void* address);


inline void* operator new(size_t size) noexcept {return malloc(size);}
inline void* operator new[](size_t size) noexcept {return malloc(size);}
inline void operator delete(void* p) noexcept {free(p);}
inline void operator delete[](void* p) noexcept {free(p);}
inline void operator delete(void* p, unsigned long _) noexcept {free(p);}