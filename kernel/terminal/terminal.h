#pragma once
#include "../lib/num.h"
#include "../lib/mem.h"
#include "../graphics/framebuffer.h"

extern u32 terminal_charX;
extern u32 terminal_charY;
extern u32 terminal_charsPerLine;

void terminal_init();
void terminal_write(char* str);
void terminal_writeln(char* str);
void terminal_writechr(char c);