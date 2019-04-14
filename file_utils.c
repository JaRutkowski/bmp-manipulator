//
// Created by rutko on 14.04.2019.
//

#include "file_utils.h"

FILE *_open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s", filename);
        exit(EXIT_FAILURE);
    }
    return fp;
}

void _handle_error(char **error, FILE *fp, BMPImage *image) {
    fprintf(stderr, "ERROR: %s\n", *error);
    _clean_up(fp, image, error);

    exit(EXIT_FAILURE);
}

BMPImage *read_image(const char *filename, char **error) {
    FILE *input_ptr = _open_file(filename, "rb");

    BMPImage *image = read_bmp(input_ptr, error);
    if (*error != NULL) {
        _handle_error(error, input_ptr, image);
    }
    fclose(input_ptr);

    return image;
}

void write_image(const char *filename, BMPImage *image, char **error) {
    FILE *output_ptr = _open_file(filename, "wb");

    if (!write_bmp(output_ptr, image, error)) {
        _handle_error(error, output_ptr, image);
    }
    fclose(output_ptr);
}

void _clean_up(FILE *fp, BMPImage *image, char **error) {
    if (fp != NULL) {
        fclose(fp);
    }
    free_bmp(image);
    free(*error);
}
