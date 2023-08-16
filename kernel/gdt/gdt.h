#pragma once
#include "../lib/num.h"
#include "../task/tss.h"

struct GDTDescriptor {
    u16 Size;
    u64 Offset;
} __attribute__((packed));

struct GDTEntry {
    u16 Limit0;
    u16 Base0;
    u8 Base1;
    u8 AccessByte;
    u8 Limit1_Flags;
    u8 Base2;
}__attribute__((packed));

struct GDT {
    struct GDTEntry Null; //0x00
    struct GDTEntry KernelCode; //0x08
    struct GDTEntry KernelData; //0x10
    struct GDTEntry UserNull;
    struct GDTEntry UserCode;
    struct GDTEntry UserData;
    struct GDTEntry TSS;
} __attribute__((packed)) 
__attribute((aligned(0x1000)));

struct GDTDescriptor* gdt_init();
extern "C" void loadGDT(struct GDTDescriptor* gdtDescriptor);
extern struct GDT defaultGDT;
extern struct GDTDescriptor gdtDescriptor;