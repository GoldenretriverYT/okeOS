#include "string.h"

char *itoa(i64 value, char *str, i64 base)
{
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10)
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

char *uitoa(u64 value, char *str, u64 base)
{
    char *rc;
    char *ptr;
    char *low;
    // Check for supported base.
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;

    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

int strncmp(char *str1, char *str2, size_t num)
{
    while (num && *str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
        --num;
    }
    if (num == 0)
    {
        return 0;
    }
    else
    {
        return (*(unsigned char *)str1 - *(unsigned char *)str2);
    }
}

FastStringBuilder::FastStringBuilder(int parts)
{
    this->parts = (char **)malloc(sizeof(char *) * (parts));
    this->nParts = parts;
}

FastStringBuilder::~FastStringBuilder()
{
    sfree(parts, sizeof(nParts));
    sfree(outputBuf, outputBufSize);
}

void FastStringBuilder::append(char *ptr)
{
    if (partOffset >= nParts)
    {
        parts[nParts-1] = "FastStringBuilder OutOfBounds";
        return;
    }

    parts[partOffset] = ptr;
    partOffset++;
}

char *FastStringBuilder::build()
{
    u64 fullSize = 0;
    u64 off = 0;

    for (int i = 0; i < nParts; i++)
    {
        fullSize += strlen((u8 *)parts[i]);
    }

    if(this->outputBuf != nullptr)
        free(this->outputBuf);

    this->outputBuf = (char *)malloc(fullSize);

    if(this->outputBuf == nullptr)
        return "FastStringBuilder Unable to allocate memory";

    for (int i = 0; i < nParts; i++)
    {
        u64 partLen = strlen((u8 *)parts[i]);
        memcpy(this->outputBuf + off, parts[i], partLen);
        off += partLen;
    }

    this->outputBufSize = fullSize;
    return this->outputBuf;
}

char *FastStringBuilder::buildAndDestroy()
{
    char *ptr = build();
    delete this;
    return ptr;
}