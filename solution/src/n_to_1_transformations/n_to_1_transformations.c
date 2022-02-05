#include "n_to_1_transformations.h"

#include <inttypes.h>
#include <stdbool.h>

#include "../util/util.h"

enum return_code apply_n_to_1_transformation(const struct image source,
                            struct image* const target,
                            transformating_function transformation) {
    const size_t width = source.width;
    const size_t height = source.height;
    *target = image_create(width, height);
    struct pixel window[WINDOW_SIDE][WINDOW_SIDE] = {0};
    for (size_t i = 0; i < height; i+=WINDOW_SIDE) {
        for (size_t j = 0; j < width; j+=WINDOW_SIDE) {
            for (uint8_t y = 0; y < WINDOW_SIDE; y++) {
                for (uint8_t x = 0; x < WINDOW_SIDE; x++) {
                    window[y][x] = image_get_pixel(source, y + i, x + j);
                }
            }
            struct pixel new_pixel = transformation(window);
            for (uint8_t y = 0; y < WINDOW_SIDE; y++) {
                for (uint8_t x = 0; x < WINDOW_SIDE; x++) {
                    image_set_pixel(*target, i + y, j + x, new_pixel);
                }
            }
        }
    }
    return SUCCESS;
}

static struct pixel blur_one(const struct pixel window[WINDOW_SIDE][WINDOW_SIDE]) {
    uint32_t sum_g = 0;
    uint32_t sum_b = 0;
    uint32_t sum_r = 0;
    for (uint8_t i = 0; i < WINDOW_SIDE; i++) {
        for (uint8_t j = 0; j < WINDOW_SIDE; j++) {
            sum_g += window[i][j].g;
            sum_b += window[i][j].b;
            sum_r += window[i][j].r;
        }
    }
    struct pixel ret = {0};
    ret.g = sum_g/WINDOW_AREA;
    ret.b = sum_b/WINDOW_AREA;
    ret.r = sum_r/WINDOW_AREA;
    return ret;
}

enum return_code apply_blur(const struct image source, struct image* const target) {
    return apply_n_to_1_transformation(source, target, blur_one);
}

static struct pixel erode_one(const struct pixel window[WINDOW_SIDE][WINDOW_SIDE]) {
    uint32_t min_g = 256;
    uint32_t min_b = 256;
    uint32_t min_r = 256;
    for (uint8_t i = 0; i < WINDOW_SIDE; i++) {
        for (uint8_t j = 0; j < WINDOW_SIDE; j++) {
            min_g = window[i][j].g < min_g ? window[i][j].g : min_g;
            min_b = window[i][j].b < min_b ? window[i][j].b : min_b;
            min_r = window[i][j].r < min_r ? window[i][j].r : min_r;
        }
    }
    struct pixel ret = {0};
    ret.g = min_g;
    ret.b = min_b;
    ret.r = min_r;
    return ret;
}

enum return_code apply_erode(const struct image source, struct image* const target) {
    return apply_n_to_1_transformation(source, target, erode_one);
}
