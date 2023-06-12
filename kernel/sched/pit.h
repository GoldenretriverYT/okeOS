#pragma once
#include "../lib/num.h"

namespace PIT {
    extern u64 timeSinceBootNs;
    const u64 baseFrequency = 1193182;

    void sleepNs(u64 ns);
    void sleepMicros(u64 micros);
    void sleepMillis(u64 millis);
    void sleepSeconds(u64 seconds);

    void setDivisor(u16 divisor);
    u64 getFrequency();
    void setFrequency(u64 frequency); 
    void tick();
}