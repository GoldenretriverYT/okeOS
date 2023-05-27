#pragma once
#include "math.h"
#include "../mem/heap.h"
#include "../debug.h"

char* itoa(i64 value, char* str, i64 base);
char* uitoa(u64 value, char* str, u64 base);
int strncmp(char* str1, char* str2, size_t num);

/// @brief The fast string builder utilizes a fixed size char* array which reduces memory overhead and amount of memory ops significantly
/// @paragraph The array itself is freed on deconstruct but not the char*s itself.
class FastStringBuilder {
    public:
    char** parts;
    int nParts;
    int partOffset = 0;

    char *outputBuf;
    u32 outputBufSize = 0;

    FastStringBuilder(int parts);
    ~FastStringBuilder();

    void append(char* ptr);
    char* build();
    char* buildAndDestroy();
};