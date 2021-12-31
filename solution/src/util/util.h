#ifndef _UTIL
#define _UTIL

enum return_code {
    SUCCESS,
    FILE_READ_ERROR,
    BMP_ERROR ,
    FILE_WRITE_ERROR,
    UNABLE_TO_OPEN_FILE,
    UNABLE_TO_CLOSE_FILE
};

void print_captioned_status(const char* const caption, const enum return_code code);
#endif
