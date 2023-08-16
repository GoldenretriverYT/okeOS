#include "gdt.h"

struct TSSDescriptor {
    u16 limit_low;
    u16 base_low;
    u8 base_mid;
    u8 type : 4;
    u8 zero : 1;
    u8 dpl : 2;
    u8 p : 1;
    u8 limit_high : 4;
    u8 avl : 1;
    u8 l : 1;
    u8 db : 1;
    u8 g : 1;
    u8 base_high;
    u32 base_upper;
    u32 reserved;
} __attribute__((packed));

#define TSS_ENTRY(base, limit) { \
    (u16)((limit) & 0xFFFF), \
    (u16)((base) & 0xFFFF), \
    (u8)(((base) >> 16) & 0xFF), \
    (u8)(0x9), \
    (u8)(0x0), \
    (u8)(0x0), \
    (u8)(0x1), \
    (u8)(((limit) >> 16) & 0x0F), \
    (u8)(0x0), \
    (u8)(0x0), \
    (u8)(0x0), \
    (u8)(0x1), \
    (u8)(((base) >> 24) & 0xFF), \
    (u32)(((base) >> 32) & 0xFFFFFFFF), \
    (u32)(0x00000000) \
}

__attribute__((aligned(0x1000)))
struct GDT defaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0}, // null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
    {0, 0, 0, 0x00, 0x00, 0}, // user null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
    {0, 0, 0, 0x00, 0x00, 0} // Tss, filled in later
};

struct GDTDescriptor gdtDescriptor;

struct GDTDescriptor* gdt_init() {
    // Create a variable to store the TSS descriptor
    TSSDescriptor tssDesc = TSS_ENTRY((u64)&tss, sizeof(tss));

    // Use the address of the variable instead of the macro
    defaultGDT.TSS = *(struct GDTEntry*)&tssDesc;

    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (u64)&defaultGDT;
    loadGDT(&gdtDescriptor);

    loadTSS(0x30);

    return &gdtDescriptor;
}