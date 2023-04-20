#include <stddef.h>
#include "limine.h"
#include "graphics/framebuffer.h"
#include "terminal/terminal.h"
#include "lib/num.h"
#include "lib/io.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/ints.h"
#include "mem/memutils.h"
#include "mem/pageframeallocator.h"
#include "mem/paging.h"
#include "debug.h"
#include "mem/paging.h"
#include "mem/heap.h"

#define DBG_DUMPMEMMAP

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

static volatile struct limine_kernel_address_request kaddr_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

struct IDTR idtr;

void setIdtGate(void* handler, u8 entryOffset, u8 typeAttr, u8 selector){
    struct IDTDescEntry* interrupt = (struct IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(struct IDTDescEntry));
    idt_setOffset(interrupt, (uint64_t)handler);
    interrupt->type_attr = typeAttr;
    interrupt->selector = selector;
}

void initInterrupts(u64 offset) {
    idtr.Limit = 0x0FFF;
    idtr.Offset = offset;

    setIdtGate((void*)pageFaultHandler, 0xE, IDT_TA_InterruptGate, 0x08);
    setIdtGate((void*)doubleFaultHandler, 0x8, IDT_TA_InterruptGate, 0x08);
    setIdtGate((void*)gpFaultHandler, 0xD, IDT_TA_InterruptGate, 0x08);
    setIdtGate((void*)keyboardInterruptHandler, 0x21, IDT_TA_InterruptGate, 0x08);
    setIdtGate((void*)mouseInterruptHandler, 0x2C, IDT_TA_InterruptGate, 0x08);
    setIdtGate((void*)pitInterruptHandler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm ("lidt %0" : : "m" (idtr));
}


// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
extern "C" void _start(void) {
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    gFramebuffer = framebuffer_request.response->framebuffers[0]; 

    gTerminal.init();
    kprintf_both("Initiliazed terminal, yay! Test: %d | Test 2: 0x%x | chars/line: %u | lines/screen: %u\n", 30, 0xFF00FF, gTerminal.charsPerLine, gTerminal.maxLines);
    
    kprintf("Initiliazing serial port: %d\n", serial_init());
    kprintf_both("Initialized serial port!\n");

    // Load GDT
    kprintf("Loading GDT...\n");
    struct GDTDescriptor* gdtDescriptor = gdt_init();
    kprintf("If you are reading this, we did load the GDT! hooray! GDTDescriptor Address: 0x%x\n", &gdtDescriptor);

    if(memmap_request.response == NULL) {
        panic("No memory map!");
        hcf();
    }

    struct limine_memmap_response memmap = *memmap_request.response;

#ifdef DBG_DUMPMEMMAP
    kprintf_serial("[dbg] Dumping memory map\n");
    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];
        kprintf_serial("0x%x [size = 0x%x, type = %d (%s)]\n", entry.base, entry.length, entry.type, getMemoryMappingName(entry.type));
    }
#endif

    kprintf("Initialize page frame allocator...\n");
    mem_pageframeallocator_init(memmap, hhdm_request.response);
    kprintf("[RAM] Total: %x, Free: %x, Used: %x, Reserved: %x\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM(), mem_getReservedRAM());
    kprintf("[RAM] Total: %u, Free: %u, Used: %u, Reserved: %u\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM(), mem_getReservedRAM());
    
    // Init IDT
    kprintf("Loading IDT & Interrupts...\n");
    initInterrupts((u64)mem_pageframeallocator_requestPage()); // TODO: Replace with allocated page when paging is added
    kprintf("Initiliazed IDT & Interrupts!\n");

    kprintf_both("Initialize paging!\n");
    globalPageTable = (PageTable*)mem_pageframeallocator_requestPage();
    memset(globalPageTable, 0, 0x1000);

    u64 hhdmOff = hhdm_request.response->offset;//((hhdm_request.response->offset >> 16) | 0xffff000000000000);
    
    kprintf_both("Mapping from 0x%x to 0x%x! | HHDM Offset: %x | pagetableaddr: %x | fb addr: %x | fb buffer addr: %x | kprintf_serial addr: %x | kernel offset phys: %x | kernel offset virt: %x | panic addr: %x | pf handler addr: %x | idtr addr: %x\n", hhdmOff, hhdmOff + mem_getMemoryMapSize(), hhdmOff, (u64)globalPageTable, (u64)gFramebuffer, (u64)gFramebuffer->address, &kprintf_serial, kaddr_request.response->physical_base, kaddr_request.response->virtual_base, &panic, &pageFaultHandler, &idtr);
    kprintf_both("idtr->offset: %x\n", idtr.Offset);
    u64 kAddr = 0;

    for(u64 i = 0; i < mem_getMemoryMapSize(); i += 0x1000) {
        /*if(mem_getMemoryMapForAddress(i) == "Kernel") {
            if(kAddr == 0) kAddr = kaddr_request.response->virtual_base;
            kprintf_serial("MAPPING KERNEL!!\n");
            PageTable_MapMemory(globalPageTable, (void*)(kAddr), (void*)(i), true);
            kAddr += 0x1000;
        }*/

        PageTable_MapMemory(globalPageTable, (void*)(hhdmOff + i), (void*)(i), false);
        PageTable_MapMemory(globalPageTable, (void*)(i), (void*)(i), false);
    }

    for(u64 i = kaddr_request.response->virtual_base; i < kaddr_request.response->virtual_base + 0x1000*1024; i += 0x1000) {
        PageTable_MapMemory(globalPageTable, (void*)i, (void*)(i - kaddr_request.response->virtual_base + kaddr_request.response->physical_base));
    }

    kprintf_both("Mapped memory\n");
    PageMapIndexer indexer;
    PageMapIndexer_From(&indexer, 0xffffffff800017b9);


    //u64 fbSize = ((gFramebuffer->width * gFramebuffer->height * gFramebuffer->bpp) / 8) + 0x1000;
    //mem_pageframeallocator_lockPages(gFramebuffer->address, fbSize / 0x1000 + 1);
    
    //kprintf_both("Locking up now, %x", globalPageTable);
    //for(u64 i = 0; i < 1000000000; i++){ io_wait(); }
    kprintf_both("Loading PML4 into CR3\n");
    asm("mov %0, %%cr3" : : "r" (globalPageTable));
    kprintf_serial("done paging!\n");
    kprintf_both("Done initiliazing paging!\n");

    kprintf_both("Enabling Kernel Heap (4kb)");
    void* heapAddr = mem_pageframeallocator_requestPage();
    mem_pageframeallocator_lockPages(heapAddr, 1);

    void* test = malloc(20);
    *(u8*)(test + 0) = 'h';
    *(u8*)(test + 1) = 'i';
    *(u8*)(test + 2) = ' ';
    *(u8*)(test + 3) = 'm';
    *(u8*)(test + 3) = 'a';
    *(u8*)(test + 3) = 'l';
    *(u8*)(test + 3) = 'l';
    *(u8*)(test + 3) = 'o';
    *(u8*)(test + 3) = 'c';

    kprintf((char*)test);

    free(test);
    
    // We're done, just hang...
    hcf();
}
