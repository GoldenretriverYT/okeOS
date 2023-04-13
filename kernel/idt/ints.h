#pragma once
#include "../lib/num.h"
#include "../panic.h"

struct interrupt_frame;
__attribute__((interrupt)) void pageFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void doubleFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void gpFaultHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void keyboardInterruptHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void mouseInterruptHandler(struct interrupt_frame* frame);
__attribute__((interrupt)) void pitInterruptHandler(struct interrupt_frame* frame);