#pragma once
#include "../lib/num.h"
#include "../panic.h"
#include "../debug.h"

enum ReservedVectors {
    DivideByZero = 0,
    Debug = 1,
    NonMaskableInterrupt = 2,
    Breakpoint = 3,
    Overflow = 4,
    BoundRangeExceeded = 5,
    InvalidOpcode = 6,
    DeviceNotAvailable = 7,
    DoubleFault = 8,
    CoprocessorSegmentOverrun = 9,
    InvalidTSS = 10,
    SegmentNotPresent = 11,
    StackSegmentFault = 12,
    GeneralProtectionFault = 13,
    PageFault = 14,
    Reserved15 = 15,
    x87FloatingPointException = 16,
    AlignmentCheck = 17,
    MachineCheck = 18,
    SIMDException = 19,
    // we dont add the unused ones here, we dont really
};

struct InterruptDescriptor
{
    u16 addressLow;
    u16 selector;
    u8 ist;
    u8 flags;
    u16 addressMid;
    u32 addressHigh;
    u32 reserved;
} __attribute__((packed));

struct IDTR
{
    u16 limit;
    u64 base;
} __attribute__((packed));

extern InterruptDescriptor idt[256];

void setIdtEntry(u8 vector, void* handler, u8 dpl);
void loadIDT(void* idtAddr);
void initInts();

struct CPUStatus
{
    u64 r15;
    u64 r14;
    u64 r13;
    u64 r12;
    u64 r11;
    u64 r10;
    u64 r9;
    u64 r8;
    u64 rdx;
    u64 rcx;
    u64 rbx;
    u64 rax;

    u64 vectorNumber;
    u64 errorCode;

    u64 iret_rip;
    u64 iret_cs;
    u64 iret_flags;
    u64 iret_rsp;
    u64 iret_ss;
};

extern char fixedPageFaultString[256];

extern "C" void interruptDispatch(CPUStatus* ctx);
const char* getPageFaultString(u64 errorCode);

extern char vector_0_handler[];