#include "bmp_padding.h"

#include <stdint.h>

#include "bmp.h"

uint8_t get_padding(const uint32_t width) 
{
    const uint8_t mod = (width * sizeof(struct pixel)) % 4;
    return mod ? 4 - mod : mod;
}
