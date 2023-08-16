#pragma once
#include "../lib/num.h"
#include "pit.h"

/**
 * @class Timer
 * @brief A class that provides a simple interface to measure elapsed time.
 */
class Timer {
    public:
        /**
         * @brief The start time of the timer.
         */
        u64 startTime = 0;

        /**
         * @brief The end time of the timer.
         */
        u64 endTime = 0;

        /**
         * @brief Constructor that sets the start time to the current time.
         */
        Timer() {
            startTime = PIT::timeSinceBootNs;
        }

        /**
         * @brief Sets the start time to the current time.
         */
        void start() {
            startTime = PIT::timeSinceBootNs;
        }

        /**
         * @brief Sets the end time to the current time.
         */
        void stop() {
            endTime = PIT::timeSinceBootNs;
        }

        /**
         * @brief Returns the elapsed time in nanoseconds.
         * @return The elapsed time in nanoseconds.
         */
        u64 getElapsedNs() {
            return endTime - startTime;
        }

        /**
         * @brief Returns the elapsed time in microseconds.
         * @return The elapsed time in microseconds.
         */
        u64 getElapsedMicros() {
            return getElapsedNs() / 1000;
        }

        /**
         * @brief Returns the elapsed time in milliseconds.
         * @return The elapsed time in milliseconds.
         */
        u64 getElapsedMillis() {
            return getElapsedMicros() / 1000;
        }

        /**
         * @brief Returns the elapsed time in seconds.
         * @return The elapsed time in seconds.
         */
        u64 getElapsedSeconds() {
            return getElapsedMillis() / 1000;
        }
};