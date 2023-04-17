#include "terminal.h"
#include "../debug.h"

u32 terminal_charX = 0;
u32 terminal_charY = 0;
u32 terminal_charsPerLine = 0;
u32 terminal_maxLines = 0;
u32 terminal_color = 0xFFFFFF;
u8 buffer[128][256]; // todo: make these dynamic to reduce mem usage
u32 colorBuffer[128][256];


void terminalInit() {
    terminal_charsPerLine = ((fb->pitch/4)/8)-1;
    terminal_maxLines = (fb->height / 16)-1;

    if(terminal_charsPerLine > 256) terminal_charsPerLine = 256;
    if(terminal_maxLines > 128) terminal_maxLines = 128;
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
    buffer[terminal_charY][terminal_charX] = c;
    colorBuffer[terminal_charY][terminal_charX] = terminal_color;

    terminal_charX++;

    if(terminal_charX > terminal_charsPerLine || c == '\n') {
        terminal_charX = 0;
        terminal_charY++;

        if(terminal_charY > terminal_maxLines) { // for now, lets just clear the screen
            memset(fb->address, 0, fb->width * fb->height * fb->bpp / 8);
            terminal_charX = 0;
            terminal_charY = 0;
        }
    }
}