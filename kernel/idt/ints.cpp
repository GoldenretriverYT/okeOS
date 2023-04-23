#include "ints.h"

__attribute__((interrupt)) void pageFaultHandler(struct interrupt_frame* frame){
    kprintf_both("Error code: %d\n", frame->err_code);
    panic("Page Fault Detected");
    while(1);
}

__attribute__((interrupt)) void doubleFaultHandler(struct interrupt_frame* frame){
    panic("Double Fault Detected");
    while(1);
}

__attribute__((interrupt)) void gpFaultHandler(struct interrupt_frame* frame){
    panic("General Protection Fault Detected");
    while(1);
}

__attribute__((interrupt)) void keyboardInterruptHandler(struct interrupt_frame* frame){
    // TODO
}

__attribute__((interrupt)) void mouseInterruptHandler(struct interrupt_frame* frame){
    // TODO
}

__attribute__((interrupt)) void pitInterruptHandler(struct interrupt_frame* frame){
    // TODO
}