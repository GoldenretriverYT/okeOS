#include "bitmap.h"

bool Bitmap::get(u64 offset) {
    if(offset > this->Size * 8) return false;

    uint8_t temp = this->Buffer[offset / 8];
    temp = (temp << (offset % 8));
    temp = (temp >> 7);

    return temp != 0;
}

bool Bitmap::set(u64 offset, bool value) {
    if(offset > this->Size * 8) return false;

    uint64_t off2 = offset / 8;
    
    uint8_t bitIndexer = 0b10000000;
    bitIndexer = bitIndexer >> (offset % 8);
    this->Buffer[off2] = this->Buffer[off2] & (~bitIndexer);

    if (value)
        this->Buffer[off2] = this->Buffer[off2] | bitIndexer;

    return true;
}