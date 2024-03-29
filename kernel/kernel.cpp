#include <stddef.h>
#include "limine.h"
#include "graphics/framebuffer.h"
#include "terminal/terminal.h"
#include "lib/num.h"
#include "lib/io.h"
#include "gdt/gdt.h"
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
#include "sched/pit.h"
#include "sched/timer.h"
#include "lib/list.h"

//#define DBG_DUMPMEMMAP
//#define DBG_TESTPIT
//#define DBG_TESTLOGGERMEMLEAK
//#define DBG_TESTLISTS

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

/*void setIdtGate(void* handler, u8 entryOffset, u8 typeAttr, u8 selector){
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

    RemapPIC();

    // enable pit
    outb(PIC1_DATA, 0b11111110);
    outb(PIC2_DATA, 0b11111111);

    asm("sti");

    PIT::setDivisor(100);
}*/

void cpuid(u32 code, u32* a, u32* b, u32* c, u32* d) {
    asm volatile("cpuid"
                 : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                 : "a"(code), "c"(0));
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
extern "C" void _start(void) {
    #pragma region Init framebuffer & terminal
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    gFramebuffer = framebuffer_request.response->framebuffers[0]; 

    gTerminal.init();
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Initiliazed terminal! Test: %d | Test 2: 0x%x | chars/line: %u | lines/screen: %u\n", 30, 0xFF00FF, gTerminal.charsPerLine, gTerminal.maxLines);
    
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Initiliazing serial port: %d\n", serial_init());
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Initialized serial port!\n");
    #pragma endregion

    #pragma region Init GDT
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Loading GDT...\n");
    struct GDTDescriptor* gdtDescriptor = gdt_init();
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF If you are reading this, we did load the GDT! hooray! GDTDescriptor Address: 0x%x\n", &gdtDescriptor);
    #pragma endregion

    #pragma region Read memory map and init page frame allocator
    if(memmap_request.response == NULL) {
        panic("No memory map!");
        hcf();
    }

    struct limine_memmap_response memmap = *memmap_request.response;

    // setup memory "constants"
    kprintf_serial("$c8[Kernel/PreLogger] $c7[???]$cF Physical Base of Kernel: %x\nVirtual Base of Kernel: %x\nHHDM Offset: %x\n", kaddr_request.response->physical_base, kaddr_request.response->virtual_base, hhdm_request.response->offset);
    mem_hhdmOffset = hhdm_request.response->offset;
    mem_kVirtOffset = kaddr_request.response->virtual_base;

#ifdef DBG_DUMPMEMMAP
    kprintf_serial("[dbg] Dumping memory map\n");
    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];
        kprintf_serial("0x%x | + hhdm 0x%x | + kvirtoff 0x%x [size = 0x%x, type = %d (%s)]\n", entry.base, hhdm_request.response->offset + entry.base, kaddr_request.response->virtual_base + entry.base, entry.length, entry.type, getMemoryMappingName(entry.type));
    }
#endif

    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Initialize page frame allocator...\n");
    mem_pageframeallocator_init(memmap, hhdm_request.response);
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Ram -> Total: %x, Free: %x, Used: %x\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM());
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Ram -> Total: %u, Free: %u, Used: %u\n", mem_getTotalRAM(), mem_getFreeRAM(), mem_getUsedRAM());
    #pragma endregion

    #pragma region Init IDT & Interrupts
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Initiliaze IDT & Interrupts...\n");
    initInts();
    kprintf("$c8[Kernel/PreLogger] $c7[???]$cF Initiliazed IDT & Interrupts!\n");
    #pragma endregion

    #pragma region Init Paging
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Initialize paging...\n");
    globalPageTable = (PageTable*)mem_pageframeallocator_requestPage();
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF PagingTable Address is %x\n", (u64)globalPageTable);
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

    // Map first 16 mb
    for(u64 i = 0; i < mb(16); i += 0x1000) {
        PageTable_MapMemory(globalPageTable, (void*)i, (void*)i);
    }

    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Mapped memory\n");
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Loading PML4 into CR3\n");
    asm("mov %0, %%cr3" : : "r" (globalPageTable));
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Done initiliazing paging!\n");
    #pragma endregion
    
    #pragma region Init Heap
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Initiliazing heap with starting size 1mb, CurrentFreeMemory: %u\n", mem_getFreeRAM());
    initHeap((void*)0x800000, mb(1) / kb(4));
    kprintf_serial("$c8[Kernel/PreLogger] $c7[???]$cF initHeap() done\n");

    char* newString = (char*)malloc(16);
    kprintf_serial("$c8[Kernel/PreLogger] $c7[???]$cF Address of newString: 0x%x\n", (u64)newString);

    char* copyable = "Hello World!";

    memcpy(newString, copyable, strlen((u8*)copyable));

    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Heap Test: %s\n", newString);
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Heap successfully initiliazed!\n");
    kprintf_both("$c8[Kernel/PreLogger] $c7[???]$cF Funny fun fact: The HeapSegHdr is %d bytes long\n", sizeof(HeapSegHdr));

    sfree(newString, 16);
    #pragma endregion

    #ifdef DBG_TESTLOGGERMEMLEAK
    Logger testLogger = Logger("Test", "Kernel");
    u64 used = getUsedMem();
    testLogger.info("Checking memory leaks from logger...");
    testLogger.info("Info from test logger!");
    testLogger.warn("Warn from test logger!");
    testLogger.error("Error from test logger!");
    testLogger.info("Before: %u; After: %u", used, getUsedMem());
    testLogger.info("Heap Test, how much memory is used for allocation of 4 bytes?");
    void* testBlockOfMemory = malloc(4);
    testLogger.info("Before: %u; After: %u", used, getUsedMem());
    sfree(testBlockOfMemory, 4);
    #endif

    #pragma region Init ACPI
    Logger acpiLogger = Logger("ACPI", "Kernel");
    acpiLogger.info("Enabling ACPI...");
    acpiLogger.info("RSDP: %x", rsdp_request.response->address);
    RSDPDescriptor* rsdp = (RSDPDescriptor*)rsdp_request.response->address;

    //rsdp->checksumValidation(); // no who gives a shat tbh then well it just isnt gonna work who cares

    if(rsdp->Revision == 2) {
        acpiLogger.warn("Detected ACPI 2.0+!"); // i dont really care about acpi 2.0+ :trol:
    }

    if(memcmp(rsdp->Signature, "RSD PTR ", 8) != 0) {
        panic("RSDP Signature does not match!");
    }

    gRSDT = (RSDT*)(void*)rsdp->RsdtAddress;
    
    acpiLogger.info("Calling ACPI::init");
    ACPI::init(&acpiLogger);
    #pragma endregion

    #ifdef DBG_TESTPIT
    Logger pitLogger = Logger("PIT", "Kernel");

    Timer timer = Timer();

    pitLogger.info("Please count the seconds. Should be 5. Also testing timer!");
    timer.start();
    PIT::sleepSeconds(5);
    timer.stop();
    pitLogger.info("Check if it was 5 seconds! According to the timer, %dms have passed", timer.getElapsedMillis());
    #endif

    #ifdef DBG_TESTLISTS
    List<u16>* list = new List<u16>();

    list->add(30);
    list->add(60);
    list->add(90);
    
    Logger listTestLogger = Logger("ListTest", "Kernel");

    listTestLogger.info("List test: %d | Expected: 30", list->get(0)); // should return 30
    listTestLogger.info("List test: %d | Expected: 60", list->get(1)); // should return 60
    listTestLogger.info("List test: %d | Expected: 90", list->get(2)); // should return 90

    list->remove((u64)1); // Since u16 is our type and we can both remove by item and by index, we need to cast to u64

    listTestLogger.info("List test: %d | Expected: 30", list->get(0)); // should return 30
    listTestLogger.info("List test: %d | Expected: 90", list->get(1)); // should return 90
    listTestLogger.info("List test: %d | Expected: 0 / NULL", list->get(2)); // should return NULL

    list->clear();
    listTestLogger.info("Lets test sorting!");
    list->add(34);
    list->add(12);
    list->add(56);
    list->add(90);
    list->add(78);

    list->sort();

    listTestLogger.info("List test: %d | Expected: 12", list->get(0)); // should return 12
    listTestLogger.info("List test: %d | Expected: 34", list->get(1)); // should return 34
    listTestLogger.info("List test: %d | Expected: 56", list->get(2)); // should return 56
    listTestLogger.info("List test: %d | Expected: 78", list->get(3)); // should return 78
    listTestLogger.info("List test: %d | Expected: 90", list->get(4)); // should return 90

    #endif

    // We're done, just hang...
    hcf();
}
