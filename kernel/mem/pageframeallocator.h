#pragma once
#include "../lib/num.h"
#include "../limine.h"
#include <stddef.h>
#include "../panic.h"
#include "memutils.h"
#include "../lib/bitmap.h"

extern u64 mem_freeMemory;
extern u64 mem_usedMemory;
extern struct Bitmap mem_pfaBitmap;

void mem_pageframeallocator_init(struct limine_memmap_response memmap, struct limine_hhdm_response* hhdm);
void mem_pageframeallocator_initBitmap(u64 bitmapSize, void* addr);

void* mem_pageframeallocator_requestPage();
void mem_pageframeallocator_lockPage(void* addr);
void mem_pageframeallocator_freePage(void* addr);
void mem_pageframeallocator_freePages(void* addr, u64 pageCount);
void mem_pageframeallocator_lockPages(void* addr, u64 pageCount);

u64 mem_getFreeRAM();
u64 mem_getUsedRAM();
u64 mem_getTotalRAM();
u64 mem_getMemoryMapSize();

char* mem_getMemoryMapForAddress(u64 addr);
