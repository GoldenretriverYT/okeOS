#pragma once
#include "../lib/num.h"
#include "../debug.h"

typedef enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // only if supported
} PT_Flag;

typedef struct PageTable { 
    PageDirectoryEntry entries [512];
}__attribute__((aligned(0x1000))) PageTable;

typedef struct PageDirectoryEntry {
    uint64_t Value;
} PageDirectoryEntry;

void PageDirectoryEntry_SetFlag(PageDirectoryEntry entry, PT_Flag flag, bool enabled);
bool PageDirectoryEntry_GetFlag(PageDirectoryEntry entry, PT_Flag flag);

void PageDirectoryEntry_SetAddress(PageDirectoryEntry entry, uint64_t address);
uint64_t PageDirectoryEntry_GetAddress(PageDirectoryEntry entry);

void PageTable_MapMemory(void* virt, void* phys);