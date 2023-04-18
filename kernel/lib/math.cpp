#include "math.h"

i64 abs(i64 val) {
    if(val < 0) return val * -1;
    return val;
}