//
// Created by rutko on 04.04.2019.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmp.h"

BMPImage *read_bmp(FILE *fp, char **error) {
    BMPImage *image = malloc(sizeof(*image));
    if (!_check(image != NULL, error, "Not enough memory")) {
        return NULL;
    }
    rewind(fp);
    int num_read = fread(&image->header, sizeof(image->header), 1, fp);
    if (!_check(num_read == 1, error, "Cannot read header")) {
        return NULL;
    }
    bool is_valid_header = check_bmp_header(&image->header, fp);
    if (!_check(is_valid_header, error, "Invalid BMP file")) {
        return NULL;
    }
    image->data = malloc(sizeof(*image->data) * image->header.image_size_bytes);
    if (!_check(image->data != NULL, error, "Not enough memory")) {
        return NULL;
    }
    num_read = fread(image->data, image->header.image_size_bytes, 1, fp);
    if (!_check(num_read == 1, error, "Cannot read image")) {
        return NULL;
    }

    return image;
}

bool write_bmp(FILE *fp, BMPImage *image, char **error) {
    rewind(fp);
    int num_read = fwrite(&image->header, sizeof(image->header), 1, fp);
    if (!_check(num_read == 1, error, "Cannot write image")) {
        return false;
    }
    num_read = fwrite(image->data, image->header.image_size_bytes, 1, fp);
    if (!_check(num_read == 1, error, "Cannot write image")) {
        return false;
    }

    return true;
}

bool check_bmp_header(BMPHeader *bmp_header, FILE *fp) {
    return
            bmp_header->type == MAGIC_VALUE
            && bmp_header->offset == BMP_HEADER_SIZE
            && bmp_header->dib_header_size == DIB_HEADER_SIZE
            && bmp_header->num_planes == NUM_PLANE
            && bmp_header->compression == COMPRESSION
            && bmp_header->num_colors == NUM_COLORS && bmp_header->important_colors == IMPORTANT_COLORS
            && bmp_header->bits_per_pixel == BITS_PER_PIXEL
            && bmp_header->size == _get_file_size(fp) &&
            bmp_header->image_size_bytes == _get_image_size_bytes(bmp_header);
}

void free_bmp(BMPImage *image) {
    free(image->data);
    free(image);
}

BMPImage *grey_filter_bmp(BMPImage *image, char **error) {
    BMPImage *new_image = malloc(sizeof(*new_image));
    new_image->header = image->header;
    new_image->header.width_px = image->header.width_px;
    new_image->header.height_px = image->header.height_px;
    new_image->header.image_size_bytes = image->header.image_size_bytes;
    new_image->header.size = BMP_HEADER_SIZE;
    new_image->data = malloc(sizeof(*new_image->data) * new_image->header.image_size_bytes);
    if (!_check(new_image->data != NULL, error, "Not enough memory")) {
        return NULL;
    }

    int new_index = 0;
    int sum;
    for (int i = 0; i < new_image->header.height_px; i++) {
        for (int j = 0; j < new_image->header.width_px; j++) {
            sum = 0;
            for (int k = 0; k < 3; k++) {
                sum += image->data[new_index];
                new_index++;
                if (k == 2) {
                    new_image->data[new_index - 2] = sum / 3;
                    new_image->data[new_index - 1] = sum / 3;
                    new_image->data[new_index] = sum / 3;
                }
            }
        }
        int padding = _get_padding(&new_image->header);
        for (int l = 0; l < padding; l++) {
            new_image->data[new_index] = 0x00;
            new_index++;
        }
    }
    return new_image;
}

/*
 * Return the size of the file.
 */
long _get_file_size(FILE *fp) {
    // Get current file position
    long current_position = ftell(fp);
    if (current_position == -1) {
        return -1;
    }
    // Set file position to the end
    if (fseek(fp, 0, SEEK_END) != 0) {
        return -2;
    }
    // Get current file position (now at the end)
    long file_size = ftell(fp);
    if (file_size == -1) {
        return -3;
    }
    // Restore previous file position
    if (fseek(fp, current_position, SEEK_SET) != 0) {
        return -4;
    }

    return file_size;
}

int _get_image_size_bytes(BMPHeader *bmp_header) {
    return _get_image_row_size_bytes(bmp_header) * bmp_header->height_px;
}

int _get_image_row_size_bytes(BMPHeader *bmp_header) {
    int bytes_per_row_without_padding = bmp_header->width_px * _get_bytes_per_pixel(bmp_header);
    return bytes_per_row_without_padding + _get_padding(bmp_header);
}

int _get_padding(BMPHeader *bmp_header) {
    return (4 - (bmp_header->width_px * _get_bytes_per_pixel(bmp_header)) % 4) % 4;
}

int _get_bytes_per_pixel(BMPHeader *bmp_header) {
    return bmp_header->bits_per_pixel / BITS_PER_BYTE;
}

bool _check(bool condition, char **error, const char *error_message) {
    bool is_valid = true;
    if (!condition) {
        is_valid = false;
        if (*error == NULL) {
            *error = _string_duplicate(error_message);
        }
    }
    return is_valid;
}

char *_string_duplicate(const char *string) {
    char *copy = malloc(sizeof(*copy) * (strlen(string) + 1));
    if (copy == NULL) {
        return "Not enough memory for error message";
    }
    strcpy(copy, string);

    return copy;
}
