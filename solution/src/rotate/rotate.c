#include "rotate.h"

#include "../image_format/image_format.h"
#include "../util/util.h"

enum return_code rotate(const struct image source, struct image* const target) {
    const size_t target_width = source.height;
    const size_t target_height = source.width;
    if (target->data != NULL) {
        image_delete(*target);
    }
    *target = image_create(target_width, target_height);
    for (size_t i = 0; i < target_height; i++)
    {
        for (size_t j = 0; j < target_width; j++)
        {
            struct pixel buffer = image_get_pixel(source, j, i);
            image_set_pixel(*target, i, target->width - j - 1, buffer);
        }
    }
    return SUCCESS;
}
