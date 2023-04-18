#pragma once
#include "../lib/num.h"
#include "../limine.h"
#include <stddef.h>
#include "../panic.h"
#include "memutils.h"
#include "../lib/bitmap.h"

extern u64 mem_freeMemory;
extern u64 mem_reservedMemory;
extern u64 mem_usedMemory;
extern struct Bitmap mem_pfaBitmap;

void mem_pageframeallocator_init(struct limine_memmap_response memmap);
void mem_pageframeallocator_initBitmap(u64 bitmapSize, void* addr);

void* mem_pageframeallocator_requestPage();

void mem_pageframeallocator_freePage(void* addr);
void mem_pageframeallocator_freePages(void* addr, u64 pageCount);

void mem_pageframeallocator_lockPage(void* addr);
void mem_pageframeallocator_lockPages(void* addr, u64 pageCount);

void mem_pageframeallocator_unreservePage(void* addr);
void mem_pageframeallocator_unreservePages(void* addr, u64 pageCount);
void mem_pageframeallocator_reservePage(void* addr);
void mem_pageframeallocator_reservePages(void* addr, u64 pageCount);

u64 mem_getFreeRAM();
u64 mem_getUsedRAM();
u64 mem_getReservedRAM();
u64 mem_getTotalRAM();

char* mem_getMemoryMapForAddress(u64 addr);
