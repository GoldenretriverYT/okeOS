#pragma once
#include "../num.h"
#include "../limine.h"

extern struct limine_framebuffer* fb;
void drawPoint(u32 x, u32 y, u32 color);

void drawLine(u32 x0, u32 y0, u32 x1, u32 y1, u32 color);
void drawHorizontalLine(u32 x0, u32 y0, u32 x1, u32 color);
void drawChar(u8 c, u32 x, u32 y, u32 fgColor);
void drawCharBg(u8 c, u32 x, u32 y, u32 fgColor, u32 bgColor);

u32 translateToOffset(uint32_t x, uint32_t y);

extern unsigned char* _binary_font_bin_start;
extern unsigned char* _binary_font_bin_end;