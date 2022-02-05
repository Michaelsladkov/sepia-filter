#include "matrix_transformations.h"

#include <inttypes.h>
#include <stdio.h>

#include "../image_format/image_format.h"
#include "../util/util.h"
#include "saturation_arithmetic.h"

static struct pixel apply_one (const struct pixel* const pixel, const float matrix[3][3]) {
    struct pixel ret;
    const struct pixel old = *pixel;
    ret.b = sat(old.b * matrix[0][0] + old.g * matrix[0][1] + old.r * matrix[0][2]);
    ret.g = sat(old.b * matrix[1][0] + old.g * matrix[1][1] + old.r * matrix[1][2]);
    ret.r = sat(old.b * matrix[2][0] + old.g * matrix[2][1] + old.r * matrix[2][2]);
    return ret;
}

enum return_code apply_matrix_transformation(const struct image source, struct image* const target, const float matrix[3][3]) {
    if (target->data == NULL) image_delete(*target);
    const size_t width = source.width;
    const size_t height = source.height;
    *target = image_create(width, height);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            const struct pixel new_pixel = apply_one(source.data + i * width + j, matrix);
            image_set_pixel(*target, i, j, new_pixel);
        }
    }
    return SUCCESS;
}

enum return_code apply_less_blue(const struct image source, struct image * const target) {
    const float matrix[3][3] = {
        {0.3, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    return apply_matrix_transformation(source, target, matrix);
}

enum return_code apply_negative(const struct image source, struct image * const target) {
    const float matrix[3][3] = {
        {-1, 0, 0},
        {0, -1, 0},
        {0, 0, -1}
    };
    return apply_matrix_transformation(source, target, matrix);
}