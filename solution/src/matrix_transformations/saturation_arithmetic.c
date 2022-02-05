#include "saturation_arithmetic.h"

#include <stdio.h>

uint8_t sat(int64_t value) {
    //fprintf (stderr, "%ld\n", value);
    if (value >= 0) return value < 255 ? value : 255;
    else {
        return 256 - sat((-1) * value);
    }
}
