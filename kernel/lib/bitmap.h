#pragma once
#include "num.h"
#include "../debug.h"

struct Bitmap {
    u64 Size;
    u8* Buffer;

    bool get(u64 offset);
    bool set(u64 offset, bool value);
};