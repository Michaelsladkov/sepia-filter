#include "file_works/file_works.h"
#include "image_format/image_format.h"
#include "sepia/sepia.h"

int main(int argc, char **argv)
{
    struct image im1 = {0}, im2 = {0};
    apply_sepia(im1, &im2);

    if (argc < 3) {
        fprintf(stderr, "not enough args\n");
        return 0;
    }
    const char* const input_file_name = argv[1];
    const char* const output_file_name = argv[2];
    struct image input_image = {0};
    const enum return_code image_read_status = image_read_file_bmp(input_file_name, &input_image);
    print_captioned_status("image reading status: ", image_read_status);
    if (image_read_status != SUCCESS) return 0;
    struct image transformed_image = {0};
    apply_sepia(input_image, &transformed_image);
    const enum return_code image_write_status = image_write_file_bmp(output_file_name, transformed_image);
    print_captioned_status("image writing status: ", image_write_status);
    image_delete(input_image);
    image_delete(transformed_image);
    return 0;
}
