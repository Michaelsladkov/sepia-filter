#include "file_works.h"

#include <errno.h>
#include <stdio.h>

#include "../bmp/bmp.h"
#include "../image_format/image_format.h"
#include "../util/util.h"

enum return_code image_read_file_bmp (const char* const filename, struct image* const image) 
{
    FILE * const fp = fopen(filename, "rb");
    if (errno != 0) return UNABLE_TO_OPEN_FILE;
    const enum return_code image_read_result = image_read_bmp(image, fp);
    if (image_read_result != SUCCESS) return image_read_result;
    fclose(fp);
    if (errno != 0) return UNABLE_TO_CLOSE_FILE;
    return SUCCESS;
}

enum return_code image_write_file_bmp(const char* const filename, const struct image image) {
    FILE * const fp = fopen(filename, "wb");
    if (errno != 0) return UNABLE_TO_OPEN_FILE;
    const enum return_code image_write_result = image_write_bmp(image, fp);
    if (image_write_result != SUCCESS) return image_write_result;
    fclose(fp);
    if (errno !=0) return UNABLE_TO_CLOSE_FILE;
    return SUCCESS;
}
