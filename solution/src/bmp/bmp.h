#ifndef _BMP
#define _BMP

#include <stdio.h>

#include "../image_format/image_format.h"
#include "../util/util.h"
enum return_code image_read_bmp(struct image* const res, FILE* const file);
enum return_code image_write_bmp(const struct image image, FILE* const file);

#endif
