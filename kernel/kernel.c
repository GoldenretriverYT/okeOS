#include <stddef.h>
#include "limine.h"
#include "graphics/framebuffer.h"
#include "terminal/terminal.h"
#include "lib/num.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/ints.h"
#include "mem/memutils.h"
#include "mem/pageframeallocator.h"
#include "mem/paging.h"
#include "debug.h"

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
void _start(void) {
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    fb = framebuffer_request.response->framebuffers[0]; 

    terminalInit();
    kprintf("Initiliazed terminal, yay! Test: %d | Test 2: 0x%x\n", 30, 0xFF00FF);
    
    // Load GDT
    kprintf("Loading GDT...\n");
    struct GDTDescriptor* gdtDescriptor = gdt_init(&defaultGDT);
    kprintf("If you are reading this, we did load the GDT! hooray! GDTDescriptor Address: 0x%x\n", &gdtDescriptor);

    // Init IDT
    kprintf("Loading IDT & Interrupts...\n");
    initInterrupts(gdtDescriptor->Offset + 0x1000); // TODO: Replace with allocated page when paging is added
    kprintf("Initiliazed IDT & Interrupts!\n");

    kprintf("[dbg] Dumping memory map\n");
    if(memmap_request.response == NULL) {
        panic("No memory map!");
        hcf();
    }

    struct limine_memmap_response memmap = *memmap_request.response;

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];
        kprintf("0x%x [size = 0x%x, type = %d (%s)]\n", entry.base, entry.length, entry.type, getMemoryMappingName(entry.type));
    }

    kprintf("Initialize page frame allocator...\n");
    mem_pageframeallocator_init(memmap);
    kprintf("[RAM] Total: %x, Free: %x, Used: %x, Reserved: %x\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM(), mem_getReservedRAM());
    kprintf("[RAM] Total: %u, Free: %u, Used: %u, Reserved: %u\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM(), mem_getReservedRAM());
    
    test();

    // We're done, just hang...
    hcf();
}
