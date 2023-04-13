#include "panic.h"

void panic(char* msg) {
    terminal_color = 0xFF0000;
    kprintf("\n\n");
    kprintf("** okeOS Panic **\n");
    kprintf("A fatal exception occurred and okeOS was halted.\n");
    kprintf("Message: %s\n", msg);
}