#include "math.h"

i32 abs(i32 val) {
    if(val < 0) return val * -1;
    return val;
}