#include "terminal.h"
#include "../debug.h"

void Terminal::init() {
    this->charsPerLine = ((gFramebuffer->pitch/4)/8)-1;
    this->maxLines = (gFramebuffer->height / 16)-1;

    if(this->charsPerLine > 256) this->charsPerLine = 256;
    if(this->maxLines > 128) this->maxLines = 128;
}

void Terminal::write(char* str) {
    while(*str != '\0') {
        writechar(*str);
        str++;
    }
}

void Terminal::writeln(char* str) {
    while(*str != '\0') {
        writechar(*str);
        str++;
    }

    writechar('\n');
}

void Terminal::writechar(char c) {
    if(c > 31) drawChar(c, this->charX * 8, this->charY * 16, this->fgColor);

    this->charX++;

    if(this->charX > this->charsPerLine || c == '\n') {
        this->charX = 0;
        this->charY++;

        if(this->charY > this->maxLines) { // TODO: Implement proper scrolling
            kprintf_serial("clear");
            memset(gFramebuffer->address, 0, gFramebuffer->width * gFramebuffer->height * gFramebuffer->bpp / 8);
            this->charX = 0;
            this->charY = 0;
        }
    }
}
Terminal gTerminal;