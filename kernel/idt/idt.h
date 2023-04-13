#pragma once
#include "../lib/num.h"

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct IDTDescEntry {
    u16 offset0; 
    u16 selector;  
    u8 ist;
    u8 type_attr;
    u16 offset1;
    u32 offset2;
    u32 ignore;
};

void idt_setOffset(struct IDTDescEntry* entry, u64 offset);
u64 idt_getOffset(struct IDTDescEntry* entry);

struct IDTR {
    u16 Limit;
    u64 Offset;
} __attribute__((packed));