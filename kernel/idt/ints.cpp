#include "ints.h"

__attribute__((interrupt)) void pageFaultHandler(struct interrupt_frame* frame){
    kprintf_serial("Error code: %d\n", frame->err_code);
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
    PIT::tick();
    PIC_EndMaster();
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
   

void RemapPIC(){
    uint8_t a1, a2; 

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);

}