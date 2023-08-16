#pragma once
#include "../lib/num.h"

struct TSSEntry
{
   u32 prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
   u64 rsp0;       // The stack pointer to load when we change to kernel mode.
   u64 rsp1;       // The stack pointer to load when we change to ring 1.
   u64 rsp2;       // The stack pointer to load when we change to ring 2.
   u64 reserved0;  // Reserved
   u64 ist[7];     // The interrupt-stack-table (IST) pointers
   u64 reserved1;  // Reserved
   u16 reserved2;  // Reserved
   u16 iomap_base; // The offset of the I/O bitmap from the start of this TSS
} __attribute__((packed));

void loadTSS(uint16_t sel);

extern TSSEntry tss;