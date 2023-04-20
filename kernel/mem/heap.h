#pragma once
#include "../lib/num.h"
#include <stddef.h>

void heap_init(void *mem, u64 size);
void* malloc(u64 size);
void free(void* mem);