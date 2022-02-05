#include "sepia.h"

#include <inttypes.h>
#include <stdio.h>

#include "../matrix_transformations/matrix_transformations.h"
#include "../matrix_transformations/saturation_arithmetic.h"
#include "../util/util.h"

static const float sepia_matrix[3][3] = {
        {0.272f, 0.543f, 0.131},
        {0.349f, 0.686f, 0.168f},
        {0.393f, 0.769f, 0.189f}
    };

static struct pixel sepia_one (const struct pixel* const pixel) {
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
    return apply_matrix_transformation(source, target, sepia_matrix);
}
