#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled) {
    u64 bitSelector = (uint64_t)1 << flag;
    this->value &= ~bitSelector;
    if (enabled){
        this->value |= bitSelector;
    }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag) {
    u64 bitSelector = (u64) 1 << flag;
    return ((this->value & bitSelector) > 0);
}

void PageDirectoryEntry::SetAddress(uint64_t address) {
    address &= 0x000000ffffffffff;
    value &= 0xfff0000000000fff;
    value |= (address << 12);
}

uint64_t PageDirectoryEntry::GetAddress() {
    return (value & 0x000ffffffffff000) >> 12;
}

void PageMapIndexer_From(PageMapIndexer* pmi, uint64_t addr) {
    addr >>= 12;
    pmi->P_i = addr & 0x1ff;
    addr >>= 9;
    pmi->PT_i = addr & 0x1ff;
    addr >>= 9;
    pmi->PD_i = addr & 0x1ff;
    addr >>= 9;
    pmi->PDP_i = addr & 0x1ff;
}

void PageTable_MapMemory(PageTable* PML4, void* virt, void* phys, bool debug) {
    PageMapIndexer indexer;
    PageMapIndexer_From(&indexer, (u64)virt);

    if(debug) {
        kprintf_serial("Mapping virt %x to phys %x (or %x). Indexer: PDP %u | PD %u | PT %u | P %u\n", virt, phys, (u64)phys >> 12, indexer.PDP_i, indexer.PD_i, indexer.PT_i, indexer.P_i);
    }
    PageDirectoryEntry PDE;

    PDE = PML4->entries[indexer.PDP_i];
    PageTable* PDP;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PDP = (PageTable*)mem_pageframeallocator_requestPage();
        memset(PDP, 0, 0x1000);
        PDE.SetAddress((uint64_t)PDP >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDE.SetFlag(PT_Flag::NX, false);
        PML4->entries[indexer.PDP_i] = PDE;
        
        if(!PML4->entries[indexer.PDP_i].GetFlag(PT_Flag::Present)) {
            panic("mapping failure");
            while(1);
        }

        kprintf_serial("Mapped PDP %d to %x", indexer.PDP_i, PDE.GetAddress() << 12);
    }
    else
    {
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
    }
    
    
    PDE = PDP->entries[indexer.PD_i];
    PageTable* PD;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PD = (PageTable*)mem_pageframeallocator_requestPage();
        memset(PD, 0, 0x1000);
        PDE.SetAddress((uint64_t)PD >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDE.SetFlag(PT_Flag::NX, false);
        PDP->entries[indexer.PD_i] = PDE;

        if(!PDP->entries[indexer.PD_i].GetFlag(PT_Flag::Present)) {
            panic("mapping failure");
            while(1);
        }
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
    }

    PDE = PD->entries[indexer.PT_i];
    PageTable* PT;
    if (!PDE.GetFlag(PT_Flag::Present)){
        PT = (PageTable*)mem_pageframeallocator_requestPage();
        memset(PT, 0, 0x1000);
        PDE.SetAddress((uint64_t)PT >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDE.SetFlag(PT_Flag::NX, false);
        PD->entries[indexer.PT_i] = PDE;

        if(!PD->entries[indexer.PT_i].GetFlag(PT_Flag::Present)) {
            panic("mapping failure");
            while(1);
        }
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
    }

    PDE = PT->entries[indexer.P_i];
    PDE.SetAddress((uint64_t)phys >> 12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    PDE.SetFlag(PT_Flag::NX, false);
    PT->entries[indexer.P_i] = PDE;

    if(!PT->entries[indexer.P_i].GetFlag(PT_Flag::Present)) {
        panic("mapping failure");
        while(1);
    }
    if(debug) kprintf_serial("Mapped to %x\n", PT->entries[indexer.P_i].GetAddress() << 12);
}

PageTable* globalPageTable;