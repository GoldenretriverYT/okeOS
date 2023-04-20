#include "pageframeallocator.h"

u64 mem_freeMemory = 0;
u64 mem_reservedMemory = 0;
u64 mem_usedMemory = 0;
u64 bitmapIdx = 0;
u64 end = 0;
struct Bitmap mem_pfaBitmap;
struct limine_memmap_response __memmap;

void mem_pageframeallocator_init(struct limine_memmap_response memmap, struct limine_hhdm_response* hhdm){
    __memmap = memmap;

    void* largestFreeSegment = NULL;
    u64 largestFreeSegmentSize = 0;

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];

        // type 0 is free memory
        if(entry.type == 0 && entry.length > largestFreeSegmentSize) {
            largestFreeSegment = (void*)entry.base;
            largestFreeSegmentSize = entry.length;
        }

        if(entry.base + entry.length > end) end = entry.base + entry.length;
    }

    if(largestFreeSegment == NULL) {
        panic("No free memory found!");
    }

    kprintf("Largest Free Segment: 0x%x (with size 0x%x)\n", largestFreeSegment, largestFreeSegmentSize);

    u64 totalSize = getMemorySize(memmap);
    kprintf("TotalSize: %x\n", totalSize);
    mem_freeMemory = totalSize;
    
    u64 bitmapSize = (totalSize / 4096 / 8) + 1;

    mem_pageframeallocator_initBitmap(bitmapSize, largestFreeSegment);
    mem_pageframeallocator_reservePages(0, (totalSize / 4096)+1);

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];

        // type 0 is free memory
        if(entry.type == 0) {
            mem_pageframeallocator_unreservePages((void*)entry.base, (entry.length/4096)+1);
        }
    }

    mem_pageframeallocator_reservePages(0, 0x100);
    mem_pageframeallocator_lockPages(mem_pfaBitmap.Buffer, (mem_pfaBitmap.Size / 4096)+1);
}

void mem_pageframeallocator_initBitmap(u64 bitmapSize, void* addr) {
    mem_pfaBitmap.Size = bitmapSize;
    mem_pfaBitmap.Buffer = (u8*)addr;

    for(int i = 0; i < bitmapSize; i++) *(u8*)((u64)mem_pfaBitmap.Buffer + i) = 0;
    bitmapIdx = 0;
}

void* mem_pageframeallocator_requestPage() {
    //kprintf_both("Page requested from 0x%x %d\n", __builtin_return_address(0), mem_freeMemory);
    if(mem_freeMemory < 16384) {
        panic("out of memory");
    }

    for(; bitmapIdx < mem_pfaBitmap.Size * 8; bitmapIdx++) {
        if(mem_pfaBitmap.get(bitmapIdx) == true) continue;
        mem_pageframeallocator_lockPage((void*)(bitmapIdx * 4096));
        return (void*)(bitmapIdx * 4096);
    }

    panic("out of memory");
}

void mem_pageframeallocator_freePage(void* addr) {
    u64 index = (u64)addr / 4096;
    if (mem_pfaBitmap.get(index) == false) return;

    if (mem_pfaBitmap.set(index, false)){
        mem_freeMemory += 4096;
        mem_usedMemory -= 4096;

        if (bitmapIdx > index) bitmapIdx = index;
    }else {
        kprintf_serial("WARN Failed to free page at address %x\n\n", (u64)addr);
    }
}

void mem_pageframeallocator_freePages(void* addr, u64 pageCount){
    for (u64 i = 0; i < pageCount; i++){
        mem_pageframeallocator_freePage((void*)((u64)addr + (i * 4096)));
    }
}

void mem_pageframeallocator_lockPage(void* addr){
    u64 index = (u64)addr / 4096;
    if (mem_pfaBitmap.get(index) == true) return;
    if (mem_pfaBitmap.set(index, true)) {
        mem_freeMemory -= 4096;
        mem_usedMemory += 4096;
    }else {
        kprintf_serial("WARN Failed to lock page at address %x (idx: %x, bitmap size: %x)\n", (u64)addr, index, mem_pfaBitmap.Size);
    }
}

void mem_pageframeallocator_lockPages(void* addr, u64 pageCount){
    for (int i = 0; i < pageCount; i++){
        mem_pageframeallocator_lockPage((void*)((u64)addr + (i * 4096)));
    }
}

void mem_pageframeallocator_unreservePage(void* addr){
    u64 index = (u64)addr / 4096;
    if (mem_pfaBitmap.get(index) == false) return;
    if (mem_pfaBitmap.set(index, false)){
        mem_freeMemory += 4096;
        mem_reservedMemory -= 4096;
        if (bitmapIdx > index) bitmapIdx = index;
    }else {
        kprintf_serial("WARN Failed to unreserve page at address %x\n", (u64)addr);
    }
}

void mem_pageframeallocator_unreservePages(void* addr, u64 pageCount){
    for (int i = 0; i < pageCount; i++){
        mem_pageframeallocator_unreservePage((void*)((u64)addr + (i * 4096)));
    }
}

void mem_pageframeallocator_reservePage(void* addr){
    u64 index = (u64)addr / 4096;
    if (mem_pfaBitmap.get(index) == true) return;
    if (mem_pfaBitmap.set(index, true)){
        mem_freeMemory -= 4096;
        mem_reservedMemory += 4096;
    }else {
        kprintf_serial("WARN Failed to reserve page at address %x\n", (u64)addr);
    }
}

void mem_pageframeallocator_reservePages(void* addr, u64 pageCount){
    for (int i = 0; i < pageCount; i++){
        mem_pageframeallocator_reservePage((void*)((u64)addr + (i * 4096)));
    }
}

u64 mem_getFreeRAM() {
    return mem_freeMemory;
}

u64 mem_getUsedRAM() {
    return mem_usedMemory;
}

u64 mem_getReservedRAM() {
    return mem_reservedMemory;
}

u64 mem_getTotalRAM() {
    //return end;
    return mem_freeMemory + mem_usedMemory + mem_reservedMemory;
}

u64 mem_getMemoryMapSize() {
    return end;
}

char* mem_getMemoryMapForAddress(u64 addr) {
    for(u64 i = 0; i < __memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*__memmap.entries)[i];

        if(addr > entry.base && addr < entry.base + entry.length) {
            return getMemoryMappingName(entry.type);
        }
    }

    return "No mapping found";
}