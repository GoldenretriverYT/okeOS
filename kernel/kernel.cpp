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
#include "acpi/rsdp.h"
#include "acpi/rsdt.h"
#include "acpi/acpi.h"
#include "lib/logger.h"

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

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
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

    // setup memory "constants"
    kprintf_serial("Physical Base of Kernel: %x\nVirtual Base of Kernel: %x\nHHDM Offset: %x\n", kaddr_request.response->physical_base, kaddr_request.response->virtual_base, hhdm_request.response->offset);
    mem_hhdmOffset = hhdm_request.response->offset;
    mem_kVirtOffset = kaddr_request.response->virtual_base;

#ifdef DBG_DUMPMEMMAP
    kprintf_serial("[dbg] Dumping memory map\n");
    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];
        kprintf_serial("0x%x | + hhdm 0x%x | + kvirtoff 0x%x [size = 0x%x, type = %d (%s)]\n", entry.base, hhdm_request.response->offset + entry.base, kaddr_request.response->virtual_base + entry.base, entry.length, entry.type, getMemoryMappingName(entry.type));
    }
#endif

    kprintf("Initialize page frame allocator...\n");
    mem_pageframeallocator_init(memmap, hhdm_request.response);
    kprintf("[RAM] Total: %x, Free: %x, Used: %x\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM());
    kprintf("[RAM] Total: %u, Free: %u, Used: %u\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM());
    
    // Init IDT
    kprintf("Loading IDT & Interrupts...\n");
    u64 idtrOffset = (u64)mem_pageframeallocator_requestPage();
    kprintf_both("Putting IDTR at address %x\n", idtrOffset);
    initInterrupts(idtrOffset);
    kprintf("Initiliazed IDT & Interrupts!\n");

    
    kprintf_both("Initialize paging!\n");
    globalPageTable = (PageTable*)mem_pageframeallocator_requestPage();
    kprintf_both("PagingTable Address is %x\n", (u64)globalPageTable);
    memset(globalPageTable, 0, 0x1000);

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];
        
        u64 sizeInPages = entry.length / 4096;
        if(entry.length % 4096 != 0) sizeInPages++;

        for(u64 pageIdx = 0; pageIdx < sizeInPages; pageIdx++) {
            u64 physAddr = entry.base + (pageIdx * 4096);
            u64 virtAddr = toVirt(physAddr);

            if(entry.type == LIMINE_MEMMAP_KERNEL_AND_MODULES) {
                virtAddr = mem_kVirtOffset + (pageIdx * 4096);
                //kprintf_serial("KERNMAP: virtual 0x%x to physical 0x%x\n", virtAddr, physAddr);
            }

            PageTable_MapMemory(globalPageTable, (void*)virtAddr, (void*)physAddr);
        }
    }

    /*// We have to map these physical locations :shrug:
    PageTable_MapMemory(globalPageTable, (void*)idtrOffset, (void*)idtrOffset);
    PageTable_MapMemory(globalPageTable, (void*)globalPageTable, (void*)globalPageTable);*/

    // Map first 16 mb
    for(u64 i = 0; i < mb(16); i += 0x1000) {
        PageTable_MapMemory(globalPageTable, (void*)i, (void*)i);
    }

    kprintf_both("Mapped memory\n");

    //u64 fbSize = ((gFramebuffer->width * gFramebuffer->height * gFramebuffer->bpp) / 8) + 0x1000;
    //mem_pageframeallocator_lockPages(gFramebuffer->address, fbSize / 0x1000 + 1);
    
    //kprintf_both("Locking up now, %x", globalPageTable);
    //for(u64 i = 0; i < 1000000000; i++){ io_wait(); }
    kprintf_both("Loading PML4 into CR3\n");
    asm("mov %0, %%cr3" : : "r" (globalPageTable));
    kprintf_serial("done paging!\n");
    kprintf_both("Done initiliazing paging!\n");
    
    // Init heap
    kprintf_both("Initiliazing heap with starting size 1mb, CurrentFreeMemory: %u\n", mem_getFreeRAM());
    initHeap((void*)0x800000, mb(1) / kb(4));
    kprintf_serial("initHeap() done\n");

    char* newString = (char*)malloc(16);
    kprintf_serial("Address of newString: 0x%x\n", (u64)newString);

    char* copyable = "Hello World!";

    memcpy(newString, copyable, strlen((u8*)copyable));

    kprintf_both("Heap Test: %s\n", newString);
    kprintf_both("Heap successfully initiliazed!\n");

    Logger testLogger = Logger("Test", "Kernel");
    u64 free = mem_getFreeRAM();
    testLogger.info("Checking memory leaks from logger...");
    testLogger.info("Info from test logger!");
    testLogger.warn("Warn from test logger!");
    testLogger.error("Error from test logger!");

    for(int i = 0; i < 128; i++) {
        char* chr = (char*)malloc(i+1);

        for(int x = 0; x < i; x++) {
            *(chr+x) = 'A';
        }

        chr[i] = '\n';

        kprintf_serial(chr);
    }

    testLogger.info("Before: %u; After: %u", free, mem_getFreeRAM());

    Logger acpiLogger = Logger("ACPI", "Kernel");
    acpiLogger.info("Enabling ACPI...");
    acpiLogger.info("RSDP: %x", rsdp_request.response->address);
    RSDPDescriptor* rsdp = (RSDPDescriptor*)rsdp_request.response->address;

    //rsdp->checksumValidation(); // no who gives a fuck tbh then well it just isnt gonna work who cares

    if(rsdp->Revision != 2) {
        acpiLogger.warn("ACPI Version 2.0+ is not fully supported.");
    }

    if(memcmp(rsdp->Signature, "RSD PTR ", 8) != 0) {
        panic("RSDP Signature does not match!");
    }

    gRSDT = (RSDT*)(void*)rsdp->RsdtAddress;
    
    acpiLogger.info("Calling ACPIsuinit");
    acpiLogger.info("Calling ACPIsuinit");
    acpiLogger.info("Calling ACPIsuinit");
    
    
    ACPI::init(&acpiLogger);

    // We're done, just hang...
    hcf();
}
