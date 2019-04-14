//
// Created by rutko on 14.04.2019.
//

#ifndef TUTORIALC_FILE_UTILS_H
#define TUTORIALC_FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

BMPImage *read_image(const char *filename, char **error);

void write_image(const char *filename, BMPImage *image, char **error);

void _clean_up(FILE *fp, BMPImage *image, char **error);

#endif //TUTORIALC_FILE_UTILS_H
