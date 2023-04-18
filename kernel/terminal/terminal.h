#pragma once
#include "../lib/num.h"
#include "../lib/mem.h"
#include "../lib/serial.h"
#include "../graphics/framebuffer.h"

class Terminal {
    public:
    u32 charX = 0;
    u32 charY = 0;
    u32 charsPerLine = 0;
    u32 maxLines = 0;
    u32 fgColor = 0xFFFFFF;

    void init();
    void write(char* str);
    void writeln(char* str);
    void writechar(char c);
};

extern Terminal gTerminal;