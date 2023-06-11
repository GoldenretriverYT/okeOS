#pragma once
#include "num.h"

void outb(u16 port, u8 value);
u8 inb(u16 port);

u16 inw (u16 port);

void io_wait();