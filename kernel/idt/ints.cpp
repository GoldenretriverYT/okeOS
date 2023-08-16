#include "ints.h"

InterruptDescriptor idt[256];

void setIdtEntry(u8 vector, void* handler, u8 dpl) {
    u64 handlerAddr = (u64)handler;

    InterruptDescriptor* entry = &idt[vector];
    entry->addressLow = handlerAddr & 0xFFFF;
    entry->addressMid = (handlerAddr >> 16) & 0xFFFF;
    entry->addressHigh = handlerAddr >> 32;
    entry->selector = 0x8;
    //trap gate + present + DPL
    entry->flags = 0b1110 | ((dpl & 0b11) << 5) |(1 << 7);
    //ist disabled
    entry->ist = 0;
}

void loadIDT(void* idtAddr)
{
    IDTR idtReg;
    idtReg.limit = 0xFFF;
    idtReg.base = (u64)idtAddr;
    IDTR* idtRegPtr = &idtReg;
    asm volatile("lidt %0" :: "m"(idtRegPtr));
}

void initInts() {
    for (u64 i = 0; i < 256; i++)
    setIdtEntry(i, (void*)vector_0_handler + (i * 16), 0);
}

extern "C" void interruptDispatch(CPUStatus* ctx)
{
    switch (ctx->vectorNumber)
    {
        case ReservedVectors::GeneralProtectionFault:
            panic("General Protection Fault");
            break;
        case ReservedVectors::PageFault:
            panic((char*)getPageFaultString(ctx->errorCode));
            break;
        default:
            kprintf_serial("Unexpected interrupt. Vector: %x (%d)", ctx->vectorNumber, ctx->vectorNumber);
            break;
    }
}

char fixedPageFaultString[256];

const char* getPageFaultString(u64 errorCode) {
    // Construct a string considering following bits in the bitfield
    // 0: Present - 0 = not present, 1 = present | Present means that the page table entry does exist
    // 1: Write - 0 = read, 1 = write | Write means that the page was written to
    // 2: User - 0 = supervisor, 1 = user | User means that the page was accessed in user mode
    // 3: Reserved bit violation - 0 = no violation, 1 = violation | Reserved bit violation means that a reserved bit was set to 1
    // 4: Instruction fetch - 0 = data access, 1 = instruction fetch | Instruction fetch means that the page was accessed by an instruction fetch
    // 5: Protection key violation - 0 = no violation, 1 = violation | Protection key violation means that the page was accessed with a protection key that does not match the key in the PTE
    // We dont care about the others

    u64 present = errorCode & 0b1;
    u64 write = (errorCode >> 1) & 0b1;
    u64 user = (errorCode >> 2) & 0b1;
    u64 reservedBitViolation = (errorCode >> 3) & 0b1;
    u64 instructionFetch = (errorCode >> 4) & 0b1;
    u64 protectionKeyViolation = (errorCode >> 5) & 0b1;

    u64 strOffset = 0;

    auto append = [&](const char* str) {
        u64 len = strlen((u8*)str);
        memcpy(fixedPageFaultString + strOffset, str, len);
        strOffset += len;
    };
    
    if(user == 0) {
        append("Supervisor Process ");
    }else {
        append("User Process ");
    }

    if(write == 0) {
        append("read from ");
    }else {
        append("write to ");
    }

    if(instructionFetch == 0) {
        append("data ");
    }else {
        append("instruction ");
    }

    if(present == 0) {
        append("non-present page entry");
    }else {
        append("and caused a protection fault");
    }

    return fixedPageFaultString;
}