#include "image_format.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct image image_create(const size_t width, const size_t height) 
{
    struct image new_image = {0};
    new_image.width = width;
    new_image.height = height;
    new_image.data = malloc(width * height * sizeof(struct pixel));
    return new_image;
}

void image_delete(const struct image image)
{
    free(image.data);
}

struct pixel image_get_pixel(const struct image source, const size_t row, const size_t column) 
{
    return source.data[row * source.width + column];
}

bool image_set_pixel(const struct image target, const size_t row, const size_t column, const struct pixel new_pixel)
{
    if (row < 0 || row >= target.height) return false;
    if (column < 0 || column >= target.width) return false;
    target.data[row * target.width + column] = new_pixel;
    return true;
}

size_t image_calculate_size(const struct image image) {
    return sizeof(struct pixel) * image.width * image.height;
}

struct image image_copy(const struct image image) {
    struct image ret = image_create(image.width, image.height);
    for (size_t i = 0; i < image.width * image.height; i++) {
        ret.data[i] = image.data[i];
    }
    return ret;
}
