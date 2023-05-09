#pragma once
#include "terminal/terminal.h"
#include "lib/string.h"
#include "lib/serial.h"
#include <stdarg.h>

void __write(char* str, bool serial, bool terminal);
void __writeChar(char str, bool serial, bool terminal);

void kprintf(char* str, ...);
void kprintf_both(char* str, ...);
void kprintf_serial(char* str, ...);
void vakprintf(char* str, va_list args);
void vakprintf_both(char* str, va_list args);
void vakprintf_serial(char* str, va_list args);

void _kprintf(char* str, bool serial, bool terminal, va_list args);
