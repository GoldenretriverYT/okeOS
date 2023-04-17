#include "bitmap.h"

bool bitmap_get(struct Bitmap bitmap, u64 offset) {
    if(offset > bitmap.Size * 8) return false;

    uint8_t temp = bitmap.Buffer[offset / 8];
    temp = (temp << (offset % 8));
    temp = (temp >> 7);

    return temp != 0;
}

bool bitmap_set(struct Bitmap bitmap, u64 offset, bool value) {
    if(offset > bitmap.Size * 8) return false;

    uint64_t off2 = offset / 8;
    
    uint8_t bitIndexer = 0b10000000;
    bitIndexer = bitIndexer >> (offset % 8);
    bitmap.Buffer[off2] = bitmap.Buffer[off2] & (~bitIndexer);

    if (value)
        bitmap.Buffer[off2] = bitmap.Buffer[off2] | bitIndexer;

    return true;
}