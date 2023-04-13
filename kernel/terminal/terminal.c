#include "terminal.h"

u32 terminal_charX = 0;
u32 terminal_charY = 0;
u32 terminal_charsPerLine = 0;
u32 terminal_color = 0xFFFFFF;

void terminalInit() {
    terminal_charsPerLine = ((fb->pitch/4)/8)-1;
}

void terminalWrite(char* str) {
    while(*str != '\0') {
        terminalWriteChar(*str);
        str++;
    }
}

void terminalWriteln(char* str) {
    while(*str != '\0') {
        terminalWriteChar(*str);
        str++;
    }

    terminalWriteChar('\n');
}

void terminalWriteChar(char c) {
    if(c > 31) drawChar(c, terminal_charX * 8, terminal_charY * 16, terminal_color);
    terminal_charX++;

    if(terminal_charX > terminal_charsPerLine || c == '\n') {
        terminal_charX = 0;
        terminal_charY++;
    }
}