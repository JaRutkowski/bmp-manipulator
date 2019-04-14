//
// Created by rutko on 04.04.2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "file_utils.h"

int main(void) {
    char *error = NULL;
    BMPImage *image = read_image("rgb24.bmp", &error);
    BMPImage *crop_image = grey_filter_bmp(image, &error);
    write_image("grey.bmp", crop_image, &error);

    _clean_up(NULL, image, &error);
    _clean_up(NULL, crop_image, &error);

    return EXIT_SUCCESS;
}
