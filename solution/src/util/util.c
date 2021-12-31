#include "util.h"

#include <stdint.h>
#include <stdio.h>

extern int errno;

static const char* error_meanings[] = {
        [SUCCESS] = "success",
        [FILE_READ_ERROR] = "file read error",
        [BMP_ERROR] = "Can not read bmp, maybe file is bad",
        [FILE_WRITE_ERROR] = "File write error",
        [UNABLE_TO_OPEN_FILE] = "Failde to open file, check permissions or something",
        [UNABLE_TO_CLOSE_FILE] = "Failde to close file"
    };

void print_captioned_status(const char* const caption, const enum return_code code) {
    fprintf(stderr, "%s", caption);
    if (code >= SUCCESS && code <= UNABLE_TO_CLOSE_FILE) 
    {
        fprintf(stderr, "%s", error_meanings[code]);
    }
    fprintf(stderr, "\n");
}
