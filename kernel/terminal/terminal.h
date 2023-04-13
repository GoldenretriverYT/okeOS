#pragma once
#include "../lib/num.h"
#include "../lib/mem.h"
#include "../graphics/framebuffer.h"

extern u32 terminal_charX;
extern u32 terminal_charY;
extern u32 terminal_charsPerLine;
extern u32 terminal_color;

void terminalInit();
void terminalWrite(char* str);
void terminalWriteln(char* str);
void terminalWriteChar(char c);