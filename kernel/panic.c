#include "panic.h"

struct stackframe {
  struct stackframe* rbp;
  u64 rip;
};

void traceStackTrace(u32 MaxFrames)
{
    struct stackframe *stk = __builtin_frame_address(0);

    kprintf("Stack trace:\n");
    for(u32 frame = 0; stk && frame < MaxFrames; ++frame)
    {
        // Unwind to previous stack frame
        kprintf("  0x%x     \n", stk->rip);
        stk = stk->rbp;
    }
}

void panic(char* msg) {
    terminal_color = 0xFF0000;
    kprintf("\n\n");
    kprintf("** okeOS Panic **                             :(\n");
    kprintf("A fatal exception occurred and okeOS was halted.\n");
    kprintf("Message: %s\n\n\n", msg);

    traceStackTrace(16);
}