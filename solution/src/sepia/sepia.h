#ifndef _SEPIA
#define _SEPIA

#include "../image_format/image_format.h"
#include "../util/util.h"

enum return_code apply_sepia_no_sse(const struct image source, struct image* const target);
enum return_code apply_sepia(const struct image source, struct image* const target);

#endif
