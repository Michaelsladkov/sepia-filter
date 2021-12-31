#ifndef _BMP_HEADER
#define _BMP_HEADER

#include <stdint.h>

#pragma pack(push, 1)

struct bmp_header 
{
        uint16_t signature;
        uint32_t fileSize;
        uint32_t reserved;
        uint32_t offset;
        uint32_t header_size;
        uint32_t width;
        uint32_t height;
        uint16_t planes;
        uint16_t bit_count;
        uint32_t compression;
        uint32_t image_size;
        uint32_t PPM_x;
        uint32_t PPM_Y;
        uint32_t colors_used;
        uint32_t colors_important;
};
#pragma pack(pop)

#endif
