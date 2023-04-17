#include "paging.h"

void PageDirectoryEntry_SetFlag(PageDirectoryEntry* entry, PT_Flag flag, bool enabled) {
    u64 bitSelector = (uint64_t)1 << flag;
    entry->Value &= ~bitSelector;
    if (enabled){
        entry->Value |= bitSelector;
    }
}

bool PageDirectoryEntry_GetFlag(PageDirectoryEntry* entry, PT_Flag flag) {
    u64 bitSelector = (u64) 1 << flag;
    return ((entry->Value & bitSelector) > 0);
}

void PageDirectoryEntry_SetAddress(PageDirectoryEntry* entry, uint64_t address) {
    address &= 0x000000ffffffffff;
    entry->Value &= 0xfff0000000000fff;
    entry->Value |= (address << 12);
}

uint64_t PageDirectoryEntry_GetAddress(PageDirectoryEntry* entry) {
    return (entry->Value & 0x000ffffffffff000) >> 12;
}

void PageMapIndexer_From(PageMapIndexer* pmi, uint64_t addr) {
    addr >>= 12;
    pmi->P = addr & 0x1ff;
    addr >>= 9;
    pmi->PT = addr & 0x1ff;
    addr >>= 9;
    pmi->PD = addr & 0x1ff;
    addr >>= 9;
    pmi->PDP = addr & 0x1ff;
}

void PageTable_MapMemory(PageTable* pml4, void* virt, void* phys) {
    PageMapIndexer pmi;
    PageMapIndexer_From(&pmi, (uint64_t)virt);

    PageDirectoryEntry pde;

    pde = pml4->entries[pmi.PDP];
    PageTable* pdp;

    if(!PageDirectoryEntry_GetFlag(&pde, Present)) {
        pdp = (PageTable*)mem_pageframeallocator_requestPage();
        memset(pdp, 0, 0x1000);

        PageDirectoryEntry_SetAddress(&pde, (u64)pdp >> 12);
        PageDirectoryEntry_SetFlag(&pde, Present, true);
        PageDirectoryEntry_SetFlag(&pde, ReadWrite, true);

        if(!PageDirectoryEntry_GetFlag(&pde, Present)) {
            kprintf_serial("dafuq");
        }

        pml4->entries[pmi.PDP] = pde;
    } else {
        pdp = (PageTable*)((u64)PageDirectoryEntry_GetAddress(&pde) << 12);
    }



    pde = pdp->entries[pmi.PD];
    PageTable* pd;

    if(!PageDirectoryEntry_GetFlag(&pde, Present)) {
        pd = (PageTable*)mem_pageframeallocator_requestPage();
        memset(pd, 0, 0x1000);

        PageDirectoryEntry_SetAddress(&pde, (u64)pd >> 12);
        PageDirectoryEntry_SetFlag(&pde, Present, true);
        PageDirectoryEntry_SetFlag(&pde, ReadWrite, true);
        pdp->entries[pmi.PD] = pde;
    } else {
        pd = (PageTable*)((u64)PageDirectoryEntry_GetAddress(&pde) << 12);
    }


    pde = pd->entries[pmi.PT];
    PageTable* pt;

    if(!PageDirectoryEntry_GetFlag(&pde, Present)) {
        kprintf_serial("PT %x in PD %x not found (whilst mapping virt 0x%x to phys 0x%x)\n", pmi.PT, pmi.PD, (u64)virt, (u64)phys);
        pt = (PageTable*)mem_pageframeallocator_requestPage();
        memset(pt, 0, 0x1000);

        PageDirectoryEntry_SetAddress(&pde, (u64)pt >> 12);
        PageDirectoryEntry_SetFlag(&pde, Present, true);
        PageDirectoryEntry_SetFlag(&pde, ReadWrite, true);
        pd->entries[pmi.PT] = pde;
    } else {
        pt = (PageTable*)((u64)PageDirectoryEntry_GetAddress(&pde) << 12);
    }

    pde = pt->entries[pmi.P];
    PageDirectoryEntry_SetAddress(&pde, (u64)phys >> 12);
    PageDirectoryEntry_SetFlag(&pde, Present, true);
    PageDirectoryEntry_SetFlag(&pde, ReadWrite, true);
    pt->entries[pmi.P] = pde;
}

PageTable* globalPageTable;