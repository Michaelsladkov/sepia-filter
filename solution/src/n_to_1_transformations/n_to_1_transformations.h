#ifndef _9TO1_TRANSFORMATIONS
#define _9TO1_TRANSFORMATIONS

#include "../image_format/image_format.h"
#include "../util/util.h"

#define WINDOW_SIDE 10
#define WINDOW_AREA (WINDOW_SIDE * WINDOW_SIDE)

typedef struct pixel transformating_function(const struct pixel window[WINDOW_SIDE][WINDOW_SIDE]);

enum return_code apply_9to1_transformation(const struct image source,
                            struct image* const target,
                            transformating_function transformation);

enum return_code apply_blur(const struct image source, struct image* const target);
enum return_code apply_erode(const struct image source, struct image* const target);

#endif