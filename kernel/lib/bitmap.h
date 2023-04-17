#pragma once
#include "num.h"

struct Bitmap {
    u64 Size;
    u8* Buffer;
};

bool bitmap_get(struct Bitmap bitmap, u64 offset);
bool bitmap_set(struct Bitmap bitmap, u64 offset, bool value);