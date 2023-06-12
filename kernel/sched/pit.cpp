#include "pit.h"
#include "../lib/io.h"

namespace PIT {
    u64 timeSinceBootNs = 0;
    u16 divisor = 65535;

    void sleepNs(u64 ns) {
        u64 startTime = timeSinceBootNs;

        while(timeSinceBootNs < startTime + ns) {
            asm("hlt");
        }
    }

    void sleepMicros(u64 micros) {
        sleepNs(micros * 1000);
    }

    void sleepMillis(u64 millis) {
        sleepNs(millis * 1000000);
    }

    void sleepSeconds(u64 seconds) {
        sleepNs(seconds * 1000000000);
    }

    void setDivisor(u16 divisor) {
        if(divisor < 100) divisor = 100;
        PIT::divisor = divisor;

        outb(0x40, (u8)(divisor & 0x00ff));
        io_wait();
        outb(0x40, (u8)((divisor & 0xff00) >> 8));
    }

    u64 getFrequency() {
        return baseFrequency / divisor;
    }

    void setFrequency(u64 frequency) {
        setDivisor(baseFrequency/frequency);
    }

    void tick() {
        timeSinceBootNs += 1000000000 / getFrequency();
    }
}