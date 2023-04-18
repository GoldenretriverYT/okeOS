#pragma once
#include "../lib/num.h"
#include "../limine.h"

extern struct limine_framebuffer* gFramebuffer;
void drawPoint(u32 x, u32 y, u32 color);

void drawLine(u32 x0, u32 y0, u32 x1, u32 y1, u32 color);
void drawHorizontalLine(u32 x0, u32 y0, u32 x1, u32 color);
void drawChar(u8 c, u32 x, u32 y, u32 fgColor);
void drawCharBg(u8 c, u32 x, u32 y, u32 fgColor, u32 bgColor);
void drawString(u8* str, u32 x, u32 y, u32 fgColor);
void drawStringBg(u8* str, u32 x, u32 y, u32 fgColor, u32 bgColor);

u32 translateToOffset(uint32_t x, uint32_t y);