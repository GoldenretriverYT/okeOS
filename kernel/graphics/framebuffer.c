#include "framebuffer.h"

struct limine_framebuffer *fb;

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
    int cx, cy;
    u8* glyph = _binary_font_bin_start + ((u32)c * 16);

    for (cy = 0; cy < 16; cy++)
    {
        for (cx = 0; cx < 8; cx++)
        {
            drawPoint(x + cx, y + cy, glyph[cy] & (1 << cx) ? fgColor : bgColor);
        }
    }
}

u32 translateToOffset(u32 x, u32 y)
{
    return (y * (fb->pitch / 4)) + x;
}