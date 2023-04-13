#pragma once
#include "../lib/num.h"
#include "../panic.h"

struct interrupt_frame
{
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
    u64 rss;
};

__attribute__((interrupt)) void pageFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void doubleFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void gpFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void keyboardInterruptHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void mouseInterruptHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void pitInterruptHandler(struct interrupt_frame* frame);