#include "tss.h"

TSSEntry tss;

void loadTSS(uint16_t sel)
{
    asm volatile("ltr %0" : : "r" (sel));
}