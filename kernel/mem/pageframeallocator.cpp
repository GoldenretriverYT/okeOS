#include "pageframeallocator.h"

u64 mem_freeMemory = 0;
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


    kprintf_both("Largest Free Segment: 0x%x (virt: 0x%x) (with size 0x%x)\n", largestFreeSegment, toVirt((u64)largestFreeSegment), largestFreeSegmentSize);

    // Calculate Bitmap Size
    u64 totalMemorySize = getMemoryArealSize(memmap);
    u64 totalMemorySizeInPages = (totalMemorySize / 4096);
    u64 bitmapSize = (totalMemorySize / 4096 /* Pages*/ / 8 /* 8 pages = 1 bitmap size (byte) */) + 1; // rounded up
    u64 bitmapSizeInPages = (bitmapSize / 4096) + 1; // The bitmap itself has a size we have to reserve later on.
    kprintf_both("PageFrameAllocator Bitmap will need %d bytes of RAM or %d pages\n", bitmapSize, bitmapSizeInPages);
    kprintf_both("Total memory space is %d bytes in size, or %d pages\n", totalMemorySize, totalMemorySizeInPages);

    // Initiliaze bitmap
    mem_pageframeallocator_initBitmap(bitmapSize, (void*)(mem_hhdmOffset + (u64)largestFreeSegment));

    // Lock bitmap & non-usable spaces
    mem_pageframeallocator_lockPages(0, totalMemorySizeInPages);

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];

        if(entry.type == 0) { // Type 0 = Usable
            u64 entrySizeInPages = entry.length / 4096;
            kprintf_serial("%d ", (entry.base));
            mem_pageframeallocator_freePages((void*)(entry.base), entrySizeInPages);
        }
    }

    // Only set these counters now since else they would be trash
    mem_usedMemory = 0; 
    mem_freeMemory = largestFreeSegmentSize;

    mem_pageframeallocator_lockPages(largestFreeSegment, bitmapSizeInPages);
    kprintf_both("Initiliazed page frame allocator!\n");
}

void mem_pageframeallocator_initBitmap(u64 bitmapSize, void* addr) {
    mem_pfaBitmap.Size = bitmapSize;
    mem_pfaBitmap.Buffer = (u8*)addr;

    for(int i = 0; i < bitmapSize; i++) *(u8*)((u64)mem_pfaBitmap.Buffer + i) = 0;
    bitmapIdx = 0;
}

/// @brief Request a page
/// @return Returns the physical address of the page
void* mem_pageframeallocator_requestPage() {
    if(mem_freeMemory < 16384) {
        panic("Out of memory");
    }

    for(; bitmapIdx < mem_pfaBitmap.Size * 8; bitmapIdx++) {
        if(mem_pfaBitmap.get(bitmapIdx)) continue;

        mem_pageframeallocator_lockPage((void*)(bitmapIdx * 4096));

        //kprintf_serial("reqpage: %x\n", bitmapIdx * 4096);
        return (void*)(bitmapIdx * 4096);
    }

    panic("Out of memory");
}

/// @brief Locks a page at the given physical address
/// @param addr Physical address of the page
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

/// @brief Locks page but does not fuck around with the free/used counters
/// @param addr 
void mem_pageframeallocator_lockPageNonAvailable(void* addr){
    u64 index = (u64)addr / 4096;

    if (mem_pfaBitmap.get(index) == true) return;
    if (!mem_pfaBitmap.set(index, true)) {
        kprintf_serial("WARN Failed to lock page at address %x (idx: %x, bitmap size: %x)\n", (u64)addr, index, mem_pfaBitmap.Size);
    }
}

/// @brief Frees/Unlocks a page at the given physical address
/// @param addr Physical address of the page
void mem_pageframeallocator_freePage(void* addr) {
    u64 index = (u64)addr / 4096;
    if (mem_pfaBitmap.get(index) == false) return;

    if (mem_pfaBitmap.set(index, false)){
        mem_freeMemory += 4096;
        mem_usedMemory -= 4096;

        if (bitmapIdx > index) bitmapIdx = index;
    }else {
        kprintf_serial("WARN Failed to free page at address %x (idx: %x, bitmap size: %x)\n", (u64)addr, index, mem_pfaBitmap.Size);
    }
}

/// @brief Frees x pages starting at the given physical address
/// @param addr 
/// @param pageCount 
void mem_pageframeallocator_freePages(void* addr, u64 pageCount){
    for (u64 i = 0; i < pageCount; i++){
        mem_pageframeallocator_freePage((void*)((u64)addr + (i * 4096)));
    }
}

/// @brief Locks x pages starting at the given physical address
/// @param addr 
/// @param pageCount 
void mem_pageframeallocator_lockPages(void* addr, u64 pageCount){
    for (int i = 0; i < pageCount; i++){
        mem_pageframeallocator_lockPage((void*)((u64)addr + (i * 4096)));
    }
}

/// @brief Locks pages but does not fuck around with the free/used counters
/// @param addr 
void mem_pageframeallocator_lockPagesNonAvailable(void* addr, u64 pageCount){
    for (int i = 0; i < pageCount; i++){
        mem_pageframeallocator_lockPageNonAvailable((void*)((u64)addr + (i * 4096)));
    }
}


u64 mem_getFreeRAM() {
    return mem_freeMemory;
}

u64 mem_getUsedRAM() {
    return mem_usedMemory;
}


u64 mem_getTotalRAM() {
    //return end;
    return mem_freeMemory + mem_usedMemory;
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