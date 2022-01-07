#include "sepia.h"

#include <inttypes.h>
#include <stdio.h>

#include "saturation_arithmetic.h"
#include "../util/util.h"

static struct pixel sepia_one (const struct pixel* const pixel) {
    static const float sepia_matrix[3][3] = {
        {0.1554f, 0.21f, 0.3227f},
        {0.224f, 0.3717f, 0.21f},
        {0.2653f, 0.4011f, 0.31626f}
    };
    struct pixel ret;
    const struct pixel old = *pixel;
    ret.b = sat(old.b * sepia_matrix[0][0] + old.g * sepia_matrix[0][1] + old.r * sepia_matrix[0][2]);
    ret.g = sat(old.b * sepia_matrix[1][0] + old.g * sepia_matrix[1][1] + old.r * sepia_matrix[1][2]);
    ret.r = sat(old.b * sepia_matrix[2][0] + old.g * sepia_matrix[2][1] + old.r * sepia_matrix[2][2]);
    return ret;
}

void transform_chunk(struct pixel source[static 4], struct pixel* restrict target);

enum return_code apply_sepia(const struct image source, struct image* const target) {
    struct image working_copy = image_create(source.width, source.height);
    const size_t number_of_pixels = source.height * source.width;
    for(size_t i = 0; i < number_of_pixels/4; i ++){
        transform_chunk(source.data + 4 * i, working_copy.data + 4 * i);
    }
    for(size_t i = number_of_pixels - number_of_pixels % 4; i < number_of_pixels; i ++){
        working_copy.data[i] = sepia_one(source.data + i);
    }
    if (target->data != NULL) image_delete(*target);
    *target = working_copy;

    return SUCCESS;
}

enum return_code apply_sepia_no_sse(const struct image source, struct image* const target) {
    if (target->data == NULL) image_delete(*target);
    const size_t width = source.width;
    const size_t height = source.height;
    *target = image_create(width, height);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            const struct pixel new_pixel = sepia_one(source.data + i * width + j);
            image_set_pixel(*target, i, j, new_pixel);
        }
    }
    return SUCCESS;
}
