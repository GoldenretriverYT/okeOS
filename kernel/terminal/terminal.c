#include "terminal.h"

u32 terminal_charX = 0;
u32 terminal_charY = 0;
u32 terminal_charsPerLine = 0;

void terminal_init() {
    terminal_charsPerLine = ((fb->pitch/4)/8)-1;
}

void terminal_write(char* str) {
    while(*str != '\0') {
        terminal_writechr(*str);
        str++;
    }
}

void terminal_writeln(char* str) {
    while(*str != '\0') {
        terminal_writechr(*str);
        str++;
    }

    terminal_writechr('\n');
}

void terminal_writechr(char c) {
    if(c > 31) drawChar(c, terminal_charX * 8, terminal_charY * 16, 0xFFFFFF);
    terminal_charX++;

    if(terminal_charX > terminal_charsPerLine || c == '\n') {
        terminal_charX = 0;
        terminal_charY++;
    }
}