#include "bitmap.h"

/**
 * @brief Get the value of a bit in the bitmap at a given offset.
 * 
 * @param offset The offset of the bit to get.
 * @return true If the bit is set (1), false otherwise (0).
 */
bool Bitmap::get(u64 offset)
{
    if (offset > this->Size * 8)
        return false;

    uint8_t temp = this->Buffer[offset / 8];
    temp = (temp << (offset % 8));
    temp = (temp >> 7);

    return temp != 0;
}

/**
 * @brief Set the value of a bit in the bitmap at a given offset.
 * 
 * @param offset The offset of the bit to set.
 * @param value The value to set the bit to (true for 1, false for 0).
 * @return true If the bit was successfully set, false otherwise.
 */
bool Bitmap::set(u64 offset, bool value)
{
    if (offset > this->Size * 8)
        return false;

    uint64_t off2 = offset / 8;

    uint8_t bitIndexer = 0b10000000;
    bitIndexer = bitIndexer >> (offset % 8);
    this->Buffer[off2] = this->Buffer[off2] & (~bitIndexer);

    if (value)
        this->Buffer[off2] = this->Buffer[off2] | bitIndexer;

    return true;
}