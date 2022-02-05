#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "n_to_1_transformations/n_to_1_transformations.h"
#include "file_works/file_works.h"
#include "image_format/image_format.h"
#include "matrix_transformations/matrix_transformations.h"
#include "sepia/sepia.h"

void test_sse(struct image input_image, const char* const output_file_name) {
    struct image transformed_image = {0};
    struct image transformed_image_no_sse = {0};
    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    apply_sepia(input_image, &transformed_image);

    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;
    
    const long sse_res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;

    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    apply_sepia_no_sse(input_image, &transformed_image_no_sse);

    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;
    
    const long no_sse_res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;

    const enum return_code image_write_status = image_write_file_bmp(output_file_name, transformed_image);
    image_write_file_bmp("reference.bmp", transformed_image_no_sse);
    print_captioned_status("image writing status: ", image_write_status);
    image_delete(input_image);
    image_delete(transformed_image);
    image_delete(transformed_image_no_sse);
    fprintf(stderr, "Time for sse in microseconds: %ld\n", sse_res);
    fprintf(stderr, "Time for no sse in microseconds: %ld\n", no_sse_res);
}

int main(int argc, char **argv)
{
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

    apply_erode(input_image, &transformed_image);
    struct image twice_transformed_image = {0};
    apply_negative(transformed_image, &twice_transformed_image);

    const enum return_code image_write_status = image_write_file_bmp(output_file_name, twice_transformed_image);
    print_captioned_status("image writing status: ", image_write_status);
    
    return 0;
}
