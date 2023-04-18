#pragma once
#include "num.h"
#include "../debug.h"

struct Bitmap {
    u64 Size;
    u8* Buffer;

    bool get(u64 offset);
    bool set(u64 offset, bool value);
};

bool bitmap_get(struct Bitmap* bitmap, u64 offset);
bool bitmap_set(struct Bitmap* bitmap, u64 offset, bool value);