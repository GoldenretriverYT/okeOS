#pragma once
#include "acpi.h"

struct RSDT {
  ACPISDTHeader h;
  u32 PointerToOtherSDT[];
};

extern RSDT* gRSDT;