#include "debug.h"

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

void kprintf(char* str, ...) {
    char buf[32];
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
                    itoa(va_arg(args, i64), buf, 8);
                    terminalWrite(buf);
                    break;
                case 'd':
                case 'i':
                    itoa(va_arg(args, i64), buf, 10);
                    terminalWrite(buf);

                    break;
                case 's':
                    terminalWrite(va_arg(args, char*));
                    break;
                case 'u':
                    uitoa(va_arg(args, u64), buf, 10);
                    terminalWrite(buf);

                    break;
                case 'x':
                case 'X': // TODO: Implement capital letters hexadecimal
                    uitoa(va_arg(args, u64), buf, 16);

                    u32 len = strlen(buf);
                    char newBuf[17] = { [0 ... 15] = '0', 0 };
                    memcpy(newBuf + (16 - len), buf, len);

                    terminalWrite(newBuf);
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