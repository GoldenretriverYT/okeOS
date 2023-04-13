#include "gdt.h"

__attribute__((aligned(0x1000)))
struct GDT defaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0}, // null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
    {0, 0, 0, 0x00, 0x00, 0}, // user null
    {0, 0, 0, 0x9a, 0xa0, 0}, // kernel code segment
    {0, 0, 0, 0x92, 0xa0, 0}, // kernel data segment
};

struct GDTDescriptor* gdt_init() {
    struct GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (u64)&defaultGDT;
    loadGDT(&gdtDescriptor);

    return &gdtDescriptor;
}