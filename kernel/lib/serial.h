#pragma once
#include "num.h"
#include "io.h"

#define PORT 0x3f8          // COM1
 
int serial_init();
int serial_received();
char serial_read();
int is_transmit_empty();
void serial_write(char a);
void serial_writemany(char* a);
