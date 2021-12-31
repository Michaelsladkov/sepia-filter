#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

#include "bmp_header.h"
#include "bmp_padding.h"

#include "../image_format/image_format.h"

#define MAX_PADDINGS 3

static const struct bmp_header HEADER_TEMPLATE =  {
    .signature = 19778,
    .reserved = 0,
    .offset = sizeof(struct bmp_header),
    .header_size = 40,
    .planes = 1,
    .compression = 0,
    .PPM_x = 2834,
    .PPM_Y = 2834,
    .colors_used = 0,
    .colors_important = 0,
    .bit_count = 24
};

static enum return_code header_write(const struct image image, FILE* const file);
static enum return_code pixels_write(const struct image image, FILE* const file);

enum return_code image_write_bmp(const struct image image, FILE* const file)
{
    const enum return_code header_write_status = header_write(image, file);
    if (header_write_status != SUCCESS) {
        return header_write_status;
    }
    return pixels_write(image, file);
}

static uint32_t file_calculate_size(struct image image) {
    return sizeof(struct bmp_header) + image_calculate_size(image);
}

static struct bmp_header create_header(const struct image image)
{
    const size_t image_size = image_calculate_size(image);
    struct bmp_header header = HEADER_TEMPLATE;
    header.fileSize = file_calculate_size(image);
    header.width = image.width;
    header.height = image.height;
    header.image_size = image_size;
    return header;
}

static enum return_code header_write(const struct image image, FILE* const file)
{
    const struct bmp_header header = create_header(image);
    if (fwrite(&header, sizeof(struct bmp_header), 1, file) != 1) {
        return FILE_WRITE_ERROR;
    }
    return SUCCESS;
}

static enum return_code pixels_write(const struct image image, FILE* const file)
{
    const uint8_t padding = get_padding(image.width);
    const size_t width = image.width;
    const size_t height = image.height;
    const uint8_t paddings[MAX_PADDINGS] = {0};
    for (size_t i = 0; i < height; i++) {
        if (!fwrite((image.data) + i * width, sizeof(struct pixel) * width, 1, file))
        {
            return FILE_WRITE_ERROR;
        } 
        if (!fwrite(paddings, padding, 1, file) && padding != 0) return FILE_WRITE_ERROR;
    }
    return SUCCESS;
}
