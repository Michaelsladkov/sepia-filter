#include "saturation_arithmetic.h"

uint8_t sat(uint64_t value) {
    return value < 255 ? value : 255;
}
