#include "debug.h"

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
                    terminalWriteChar((char)va_arg(args, u32));
                    break;
                case 'e':
                case 'E':
                    terminalWrite("<formatter e/E not implemented>");
                    break;
                case 'f':
                    terminalWrite("<formatter f not implemented>");
                    break;
                case 'o':
                    itoa(va_arg(args, i32), buf, 8);
                    terminalWrite(buf);
                    break;
                case 'd':
                case 'i':
                    itoa(va_arg(args, i32), buf, 10);
                    terminalWrite(buf);

                    break;
                case 's':
                    terminalWrite(va_arg(args, char*));
                    break;
                case 'u':
                    itoa(va_arg(args, u32), buf, 10);
                    terminalWrite(buf);

                    break;
                case 'x':
                case 'X': // TODO: Implement capital letters hexadecimal
                    itoa(va_arg(args, u32), buf, 16);
                    terminalWrite(buf);

                    break;
                case '%':
                    terminalWriteChar('%');
                    break;
            }

            str++;
            isParsingSpecifier = 0;
            continue;
        }

        terminalWriteChar(*str);
        str++;
    }
}