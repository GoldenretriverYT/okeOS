#pragma once
#include "../lib/num.h"
#include "../limine.h"

// Constants for limine memmap type
#define LIMINE_MEMMAP_USABLE                 0
#define LIMINE_MEMMAP_RESERVED               1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE       2
#define LIMINE_MEMMAP_ACPI_NVS               3
#define LIMINE_MEMMAP_BAD_MEMORY             4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULES     6
#define LIMINE_MEMMAP_FRAMEBUFFER            7

extern u64 mem_hhdmOffset;
extern u64 mem_kVirtOffset;
char* getMemoryMappingName(u64 type);
u64 getMemorySize(struct limine_memmap_response memmap);
u64 getMemoryArealSize(struct limine_memmap_response memmap);
u64 toPhys(u64 virt);
u64 toVirt(u64 phys);