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
    ACPI::facp = ACPI::findTable("FACP");
    FADT* fadt = (FADT*)ACPI::facp;

    logger->info((char*)fadt->h.Checksum);
}

void* ACPI::findTable(char* sig) {
    RSDT *rsdt = gRSDT;
    int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 
    for (int i = 0; i < entries; i++)
    {
        ACPISDTHeader *h = (ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
        if (!strncmp(h->Signature, "FACP", 4))
            return (void*) h;
    }
 
    // No FACP found
    return nullptr;
}