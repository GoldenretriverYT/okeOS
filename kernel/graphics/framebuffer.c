#include "framebuffer.h"

struct limine_framebuffer *fb;
extern u8 _binary_font_bin_start[];
extern u8 _binary_font_bin_end[];

void drawPoint(u32 x, u32 y, u32 color)
{
    u32 *addr = fb->address;
    addr[translateToOffset(x, y)] = color;
}

void drawLine(u32 x0, u32 y0, u32 x1, u32 y1, u32 color)
{
    if (x0 != x1)
    {
        if (y0 != y1)
            return; // Diagonal lines not supported yet

        if (x0 > x1)
            drawHorizontalLine(x1, y0, x0, color);
        else
            drawHorizontalLine(x0, y0, x1, color);
    }
}

void drawHorizontalLine(u32 x0, u32 y0, u32 x1, u32 color)
{
    u32 *addr = fb->address;
    for (u32 x = x0; x < x1; x++)
    {
        addr[translateToOffset(x, y0)] = color;
    }
}

void drawChar(u8 c, u32 x, u32 y, u32 fgColor)
{
    drawCharBg(c, x, y, fgColor, 0x000000);
}

void drawCharBg(u8 c, u32 x, u32 y, u32 fgColor, u32 bgColor)
{
    u8* glyph = _binary_font_bin_start + ((u32)c * 16);

    if(glyph+16 >= &_binary_font_bin_end) {
        for (int cy = 0; cy < 16; cy++)
        {
            for (int cx = 0; cx < 8; cx++)
            {
                drawPoint(x + cx, y + cy, 0xFF00FF);
            }
        }
        return; // holy cow how did this happen !!
    }

    for (int cy = 0; cy < 16; cy++)
    {
        for (int cx = 0; cx < 8; cx++)
        {
            if(glyph + cy >= _binary_font_bin_end) return;
            char v = (*(glyph + cy));

            char m = (1 << cx);
            char mv = v & m;
            drawPoint((x+16)-cx, y + cy, mv ? fgColor : bgColor);
        }
    }
}

void drawString(u8* str, u32 x, u32 y, u32 fgColor)
{
    drawStringBg(str, x, y, fgColor, 0x000000);
}

void drawStringBg(u8* str, u32 x, u32 y, u32 fgColor, u32 bgColor)
{
    u32 offX = 0;
    while(*str != '\0') {
        drawCharBg(*str, x + offX, y, fgColor, bgColor);
        offX += 8;
        str++;
    }
}

u32 translateToOffset(u32 x, u32 y)
{
    return (y * (fb->pitch / 4)) + x;
}