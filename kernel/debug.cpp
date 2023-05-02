#include "debug.h"

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

bool currentlyReadingAnything = false;
bool currentlyReadingColor = false;

void __write(char* str, bool serial, bool terminal) {
    
    while(*str != 0) {
        #pragma region ugly code i know
        if(*str == '$') {
            currentlyReadingAnything = true;
            continue;
        }

        if(currentlyReadingAnything) {
            if(!currentlyReadingColor) {
                if(*str == 'c') currentlyReadingColor = true;
            } else {
                if(*str == 'Y') {
                    gTerminal.fgColor = 0xFFFF20;
                }else if(*str == 'G') {
                    gTerminal.fgColor = 0x00FF00;
                }else if(*str == 'R') {
                    gTerminal.fgColor = 0xFF0000;
                }else if(*str == 'B') {
                    gTerminal.fgColor = 0x0000FF;
                }else if(*str == 'F') {
                    gTerminal.fgColor = 0xFFFFFF;
                }

                currentlyReadingAnything = false;
                currentlyReadingColor = false;
            }

            continue;
        }
        #pragma endregion

        if(serial) {
            serial_write(*str);
        }

        if(terminal) {
            gTerminal.writechar(*str);
        }
        str++;
    }
}

void __writeChar(char str, bool serial, bool terminal) {
    #pragma region ugly code i know
    if(str == '$') {
        currentlyReadingAnything = true;
        return;
    }

    if(currentlyReadingAnything) {
        if(!currentlyReadingColor) {
            if(str == 'c') currentlyReadingColor = true;
        } else {
            if(str == 'Y') {
                gTerminal.fgColor = 0xFFFF20;
            }else if(str == 'G') {
                gTerminal.fgColor = 0x00FF00;
            }else if(str == 'R') {
                gTerminal.fgColor = 0xFF0000;
            }else if(str == 'B') {
                gTerminal.fgColor = 0x0000FF;
            }else if(str == 'F') {
                gTerminal.fgColor = 0xFFFFFF;
            }

            currentlyReadingAnything = false;
            currentlyReadingColor = false;
        }

        return;
    }
    #pragma endregion

    if(serial) {
        serial_write(str);
    }

    if(terminal) {
        gTerminal.writechar(str);
    }
}

/// Formatted kernel print to only terminal
/// Supported formatters: c (char), o (octal), d/i (decimal base10), s (string), u (unsigned base10), x (unsigned base16)
/// You can use $c<Color> to print colors too! Valid colors: Y (Yellow), R (Red), G (Green), B (Blue), F (White/Reset)
void kprintf(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, false, true, args);
}

/// Formatted kernel print to both channels (terminal & serial)
/// Supported formatters: c (char), o (octal), d/i (decimal base10), s (string), u (unsigned base10), x (unsigned base16)
/// You can use $c<Color> to print colors too! Valid colors: Y (Yellow), R (Red), G (Green), B (Blue), F (White/Reset)
void kprintf_both(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, true, true, args);
}

/// Formatted kernel print to serial
/// Supported formatters: c (char), o (octal), d/i (decimal base10), s (string), u (unsigned base10), x (unsigned base16)
/// You can use $c<Color> to print colors too! Valid colors: Y (Yellow), R (Red), G (Green), B (Blue), F (White/Reset)
void kprintf_serial(char* str, ...) {
    va_list args;
    va_start(args, str);
    _kprintf(str, true, false, args);
}

/// Formatted kernel print
/// Supported formatters: c (char), o (octal), d/i (decimal base10), s (string), u (unsigned base10), x (unsigned base16)
/// You can use $c<Color> to print colors too! Valid colors: Y (Yellow), R (Red), G (Green), B (Blue), F (White/Reset)
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
                {
                    uitoa(va_arg(args, u64), buf, 16);

                    u32 len = strlen((u8*)buf);
                    char newBuf[17] = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 0 };
                    memcpy(newBuf + (16 - len), buf, len);

                    __write(newBuf, serial, terminal);
                    break;
                }
                case '%':
                {
                    __writeChar('%', serial, terminal);
                    break;
                }
            }

            str++;
            isParsingSpecifier = 0;
            continue;
        }

        __writeChar(*str, serial, terminal);
        str++;
    }
}