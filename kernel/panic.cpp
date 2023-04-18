#include "panic.h"

struct stackframe {
  struct stackframe* rbp;
  u64 rip;
};

void traceStackTrace(u32 MaxFrames)
{
    struct stackframe *stk = (stackframe*)__builtin_frame_address(0);

    kprintf("Stack trace:\n");
    for(u32 frame = 0; stk && frame < MaxFrames; ++frame)
    {
        // Unwind to previous stack frame
        kprintf("  0x%x     \n", stk->rip);
        stk = stk->rbp;
    }
}

void traceStackTraceOnlySerial(u32 MaxFrames)
{
    struct stackframe *stk = (stackframe*)__builtin_frame_address(0);

    kprintf_serial("Stack trace:\n");
    for(u32 frame = 0; stk && frame < MaxFrames; ++frame)
    {
        // Unwind to previous stack frame
        kprintf_serial("  0x%x     \n", stk->rip);
        stk = stk->rbp;
    }
}

void panic(char* msg) { // TODO: Cleanup
    terminal_color = 0xFF0000;
    kprintf_serial("Panic GODDAMN!\n");
    kprintf_serial("Message: %s\n\n\n", msg);

    traceStackTraceOnlySerial(16);

    kprintf("\n\n");
    kprintf("** okeOS Panic **                             :(\n");
    kprintf("A fatal exception occurred and okeOS was halted.\n");
    kprintf("Message: %s\n\n\n", msg);

    traceStackTrace(16);
}