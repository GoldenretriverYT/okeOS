#include "idt.h"
#include "../debug.h"

void idt_setOffset(struct IDTDescEntry* entry, u64 offset){
    entry->offset0 = (u16)(offset & 0x000000000000ffff);
    entry->offset1 = (u16)((offset & 0x00000000ffff0000) >> 16);
    entry->offset2 = (u32)((offset & 0xffffffff00000000) >> 32);
}

u64 idt_getOffset(struct IDTDescEntry* entry){ 
    u64 offset = 0;
    offset |= (u64)entry->offset0;
    offset |= (u64)entry->offset1 << 16;
    offset |= (u64)entry->offset2 << 32;
    return offset;
}