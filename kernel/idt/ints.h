#pragma once
#include "../lib/num.h"
#include "../panic.h"
#include "../lib/io.h"
#include "../sched/pit.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame
{
    u64 err_code;
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

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();