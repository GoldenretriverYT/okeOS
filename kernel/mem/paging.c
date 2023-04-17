#include "paging.h"

void PageDirectoryEntry_SetFlag(PageDirectoryEntry entry, PT_Flag flag, bool enabled) {
    u64 bitSelector = (uint64_t)1 << flag;
    entry.Value &= ~bitSelector;
    if (enabled){
        entry.Value |= bitSelector;
    }
}

bool PageDirectoryEntry_GetFlag(PageDirectoryEntry entry, PT_Flag flag) {
    u64 bitSelector = (u64)1 << flag;
    return entry.Value & bitSelector > 0 ? true : false;
}

void PageDirectoryEntry_SetAddress(PageDirectoryEntry entry, uint64_t address) {
    address &= 0x000000ffffffffff;
    entry.Value &= 0xfff0000000000fff;
    entry.Value |= (address << 12);
}

uint64_t PageDirectoryEntry_GetAddress(PageDirectoryEntry entry) {
    return (entry.Value & 0x000ffffffffff000) >> 12;
}