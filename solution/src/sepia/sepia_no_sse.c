#include "sepia.h"

#include "inttypes.h"

#include "saturation_arithmetic.h"
#include "../image_format/image_format.h"
#include "../util/util.h"

static struct pixel sepia_one (const struct pixel* const pixel) {
    static const float sepia_matrix[3][3] = {
        {0.42f, 0.7f, 0.3f},
        {0.3f, 0.6f, 0.208f},
        {0.222f, 0.3f, 0.461f}
    };
    struct pixel ret;
    const struct pixel old = *pixel;
    ret.r = sat(old.r * sepia_matrix[0][0] + old.g * sepia_matrix[0][1] + old.b * sepia_matrix[0][2]);
    ret.g = sat(old.r * sepia_matrix[1][0] + old.g * sepia_matrix[1][1] + old.b * sepia_matrix[1][2]);
    ret.b = sat(old.r * sepia_matrix[2][0] + old.g * sepia_matrix[2][1] + old.b * sepia_matrix[2][2]);
    return ret;
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
