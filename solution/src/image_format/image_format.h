#ifndef _IMAGE_FORMAT
#define _IMAGE_FORMAT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct image
{
  size_t width, height;
  struct pixel* data;
};

#pragma pack(push, 1)

struct pixel
{
    uint8_t b, g, r;
};
#pragma pack(pop)

struct image image_create(const size_t width, const size_t height);
void image_delete(const struct image image);
struct pixel image_get_pixel(const struct image source, const size_t row, const size_t column);
bool image_set_pixel(const struct image target, const size_t row, const size_t column, const struct pixel new_pixel);
size_t image_calculate_size(const struct image image);

#endif
