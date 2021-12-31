#ifndef _FILE_WORKS
#define _FILE_WORKS

#include "../image_format/image_format.h"
#include "../util/util.h"

#include <stdio.h>

enum return_code image_read_file_bmp (const char* const filename, struct image* const image);
enum return_code image_write_file_bmp (const char* const filename, const struct image image);

#endif
