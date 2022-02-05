#ifndef _MATRIX_TRANSFORMATIONS
#define _MATRIX_TRANSFORMATIONS

#include "../image_format/image_format.h"
#include "../util/util.h"

enum return_code apply_matrix_transformation(const struct image source, struct image* const target, const float matrix[3][3]);
enum return_code apply_less_blue(const struct image source, struct image * const target);
enum return_code apply_negative(const struct image source, struct image * const target);

#endif