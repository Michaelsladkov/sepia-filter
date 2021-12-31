#include "bmp.h"

#include <stdint.h>
#include <stdlib.h>

#include "bmp_header.h"
#include "bmp_padding.h"

#include "../image_format/image_format.h"
#include "../util/util.h"

static enum return_code pixels_read(FILE* const file, const struct image image);
static enum return_code header_read(FILE* const file, struct bmp_header * const header);

enum return_code image_read_bmp(struct image* const res, FILE* const file) 
{
    struct bmp_header header = {0};
    const enum return_code file_read_status = header_read(file, &header);
    if (file_read_status != SUCCESS) return file_read_status;
    const uint32_t im_width = header.width;
    const uint32_t im_height = header.height;
    *res  = image_create(im_width, im_height);
    if (fseek(file, header.offset, SEEK_SET) != 0) return BMP_ERROR;
    return pixels_read(file, *res);
}

static enum return_code header_read(FILE* const file, struct bmp_header * const header) 
{
    if (fread(header, sizeof(struct bmp_header), 1, file)) 
    {
        return SUCCESS;
    }
    return FILE_READ_ERROR;
}

static enum return_code pixels_read(FILE* const file, const struct image image) 
{
    const size_t height = image.height;
    const size_t width = image.width;
    struct pixel * const pixels = image.data;
    const uint8_t padding = get_padding(width);
    for (uint32_t i = 0; i < height; i++) {
        if (fread(pixels + width * i, sizeof(struct pixel), width, file) != width) return FILE_READ_ERROR;
        if (fseek(file, padding, SEEK_CUR) != 0) return BMP_ERROR;
    }
    return SUCCESS;
}
