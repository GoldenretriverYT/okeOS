#include "acpi.h"
#include "rsdt.h"

Logger* ACPI::logger = nullptr;
void* ACPI::facp = nullptr;

bool doChecksum(ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < tableHeader->Length; i++)
    {
        sum += ((char *) tableHeader)[i];
    }
 
    return sum == 0;
}

void ACPI::init(Logger* logger) {
    ACPI::logger = logger;
    logger->info("Looking for FACP");
    
    ACPI::facp = ACPI::findTable("FACP");

    logger->info("FACP: %x", ACPI::facp);
    if(ACPI::facp == nullptr) {
        panic("ACPI FACP was not found!");
        return;
    }

    FADT* fadt = (FADT*)ACPI::facp;

    outb(fadt->SMI_CommandPort, fadt->AcpiEnable);
    while (inw(fadt->PM1aControlBlock) & 1 == 0);

    logger->info("ACPI is (hopefully) enabled");
}

void* ACPI::findTable(char* sig) {
    RSDT *rsdt = gRSDT;
    PageTable_MapMemory(globalPageTable, rsdt, rsdt);
    int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 
    for (int i = 0; i < entries; i++)
    {
        PageTable_MapMemory(globalPageTable, (void*)rsdt->PointerToOtherSDT[i], (void*)rsdt->PointerToOtherSDT[i]);
        ACPISDTHeader *h = (ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
        if (!strncmp(h->Signature, "FACP", 4))
            return (void*) h;
    }
 
    // No FACP found
    return nullptr;
}