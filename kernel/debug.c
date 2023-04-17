#include "debug.h"

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

void __write(char* str, bool serial, bool terminal) {
    while(*str != 0) {
        if(serial) {
            serial_write(*str);
        }

        if(terminal) {
            terminalWriteChar(*str);
        }
        str++;
    }
}

void __writeChar(char str, bool serial, bool terminal) {
    if(serial) {
        serial_write(str);
    }

    if(terminal) {
        terminalWriteChar(str);
    }
}

void kprintf(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, false, true, args);
}

void kprintf_both(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, true, true, args);
}

void kprintf_serial(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, true, false, args);
}

void _kprintf(char* str, bool serial, bool terminal, va_list args) {
    char buf[32];
    u8 isParsingSpecifier = 0;
    
    while(*str != '\0') {
        if(*str == '%') {
            isParsingSpecifier = 1;
            str++;
            continue;
        }

        if(isParsingSpecifier) {
            switch(*str) {
                case 'c':
                    __writeChar((char)va_arg(args, u32), serial, terminal);
                    break;
                case 'e':
                case 'E':
                    __write("<formatter e/E not implemented>", serial, terminal);
                    break;
                case 'f':
                    __write("<formatter f not implemented>", serial, terminal);
                    break;
                case 'o':
                    itoa(va_arg(args, i64), buf, 8);
                    __write(buf, serial, terminal);
                    break;
                case 'd':
                case 'i':
                    itoa(va_arg(args, i64), buf, 10);
                    __write(buf, serial, terminal);

                    break;
                case 's':
                    __write(va_arg(args, char*), serial, terminal);
                    break;
                case 'u':
                    uitoa(va_arg(args, u64), buf, 10);
                    __write(buf, serial, terminal);

                    break;
                case 'x':
                case 'X': // TODO: Implement capital letters hexadecimal
                    uitoa(va_arg(args, u64), buf, 16);

                    u32 len = strlen(buf);
                    char newBuf[17] = { [0 ... 15] = '0', 0 };
                    memcpy(newBuf + (16 - len), buf, len);

                    __write(newBuf, serial, terminal);
                    break;
                case '%':
                    __writeChar('%', serial, terminal);
                    break;
            }

            str++;
            isParsingSpecifier = 0;
            continue;
        }

        __writeChar(*str, serial, terminal);
        str++;
    }
}