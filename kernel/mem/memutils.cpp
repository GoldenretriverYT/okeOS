#include "memutils.h"

u64 mem_hhdmOffset;
u64 mem_kVirtOffset;

char* getMemoryMappingName(u64 type) {
    switch(type) {
        case LIMINE_MEMMAP_USABLE: return "Usable Memory";
        case LIMINE_MEMMAP_RESERVED: return "Reserved Memory";
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE: return "ACPI Reclaimable";
        case LIMINE_MEMMAP_ACPI_NVS: return "ACPI NVS";
        case LIMINE_MEMMAP_BAD_MEMORY: return "[!] Bad Memory";
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return "Bootloader Reclaimable";
        case LIMINE_MEMMAP_KERNEL_AND_MODULES: return "Kernel";
        case LIMINE_MEMMAP_FRAMEBUFFER: return "Framebuffer";
    }

    return "Unknown";
}

u64 getMemorySize(struct limine_memmap_response memmap) {
    u64 size = 0;

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];

        size += entry.length;
    }

    return size;
}

u64 getMemoryArealSize(struct limine_memmap_response memmap) {
    u64 size = 0;

    for(u64 i = 0; i < memmap.entry_count; i++) {
        struct limine_memmap_entry entry = (*memmap.entries)[i];

        if(entry.base + entry.length > size) size = entry.base + entry.length;
    }

    return size;
}

u64 toPhys(u64 virt) {
    return virt - mem_hhdmOffset;
}

u64 toVirt(u64 phys) {
    return phys + mem_hhdmOffset;
}
