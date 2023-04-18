#include "io.h"

void outb(u16 port, u8 value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

u8 inb(u16 port) {
    u8 returnVal;
    asm volatile ("inb %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
}

void io_wait() {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}