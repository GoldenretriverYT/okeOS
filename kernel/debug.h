#pragma once
#include "terminal/terminal.h"
#include "lib/string.h"
#include <stdarg.h>

void kprintf(char* str, ...) {
    char buf[16];
    va_list args;
    u8 isParsingSpecifier = 0;

    va_start(args, str);
    while(*str != '\0') {
        if(*str == '%') {
            isParsingSpecifier = 1;
            str++;
            continue;
        }

        if(isParsingSpecifier) {
            switch(*str) {
                case 'c':
                    terminal_writechr((char)va_arg(args, u32));
                    break;
                case 'e':
                case 'E':
                    terminal_write("<formatter e/E not implemented>");
                    break;
                case 'f':
                    terminal_write("<formatter f not implemented>");
                    break;
                case 'o':
                    itoa(va_arg(args, i32), buf, 8);
                    terminal_write(buf);
                    break;
                case 'd':
                case 'i':
                    itoa(va_arg(args, i32), buf, 10);
                    terminal_write(buf);

                    break;
                case 's':
                    terminal_write(va_arg(args, char*));
                    break;
                case 'u':
                    itoa(va_arg(args, u32), buf, 10);
                    terminal_write(buf);

                    break;
                case 'x':
                case 'X': // TODO: Implement capital letters hexadecimal
                    itoa(va_arg(args, u32), buf, 16);
                    terminal_write(buf);

                    break;
                case '%':
                    terminal_writechr('%');
                    break;
            }

            str++;
            isParsingSpecifier = 0;
            continue;
        }

        terminal_writechr(*str);
        str++;
    }
}