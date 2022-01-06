#include "sepia.h"

#include <inttypes.h>
#include <stdio.h>

#include "saturation_arithmetic.h"
#include "../util/util.h"

static struct pixel sepia_one (const struct pixel* const pixel) {
    static const float sepia_matrix[3][3] = {
        {0.42f, 0.7f, 0.3f},
        {0.3f, 0.6f, 0.208f},
        {0.222f, 0.3f, 0.461f}
    };
    struct pixel ret;
    const struct pixel old = *pixel;
    ret.b = sat(old.b * sepia_matrix[0][0] + old.g * sepia_matrix[0][1] + old.r * sepia_matrix[0][2]);
    ret.g = sat(old.b * sepia_matrix[1][0] + old.g * sepia_matrix[1][1] + old.r * sepia_matrix[1][2]);
    ret.r = sat(old.b * sepia_matrix[2][0] + old.g * sepia_matrix[2][1] + old.r * sepia_matrix[2][2]);
    return ret;
}

void transform_chunk(struct pixel chunk[static 4]);

enum return_code apply_sepia(const struct image source, struct image* const target) {
    /*struct image working_copy = image_copy(source);
    for(size_t i = 0; i < source.height * source.width; i += 4){
        transform_chunk(working_copy.data + i);
    }
    if (target->data != NULL) image_delete(*target);
    *target = working_copy;*/
    struct pixel a[5] = {{200, 100, 50}, {10, 20 , 30}, {210, 110, 51}, {200, 100 , 50}, {10, 20, 30}};
    struct pixel b = sepia_one(a + 4);
    printf("%u %u %u\n", b.b, b.g, b.r);
    transform_chunk(a);
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
