#pragma once
#include "../lib/num.h"

struct RSDPDescriptor {
 char Signature[8];
 u8 Checksum;
 char OEMID[6];
 u8 Revision;
 u32 RsdtAddress;
} __attribute__ ((packed));

struct RSDPDescriptor20 {
 RSDPDescriptor firstPart;
 
 u32 Length;
 u64 XsdtAddress;
 u8 ExtendedChecksum;
 u8 reserved[3];
} __attribute__ ((packed));