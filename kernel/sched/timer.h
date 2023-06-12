#pragma once
#include "../lib/num.h"
#include "pit.h"

class Timer {
    public:
        u64 startTime = 0;
        u64 endTime = 0;

        Timer() {
            startTime = PIT::timeSinceBootNs;
        }

        void start() {
            startTime = PIT::timeSinceBootNs;
        }

        void stop() {
            endTime = PIT::timeSinceBootNs;
        }

        u64 getElapsedNs() {
            return endTime - startTime;
        }

        u64 getElapsedMicros() {
            return getElapsedNs() / 1000;
        }

        u64 getElapsedMillis() {
            return getElapsedMicros() / 1000;
        }

        u64 getElapsedSeconds() {
            return getElapsedMillis() / 1000;
        }
};