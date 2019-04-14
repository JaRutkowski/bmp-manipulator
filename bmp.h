//
// Created by rutko on 04.04.2019.
//

#ifndef TUTORIALC_BMP_H
#define TUTORIALC_BMP_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40

#pragma pack(push)
#pragma pack(1)

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t dib_header_size;
    int32_t width_px;
    int32_t height_px;
    uint16_t num_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size_bytes;
    int32_t x_resolution_ppm;
    int32_t y_resolution_ppm;
    uint32_t num_colors;
    uint32_t important_colors;
} BMPHeader;

#pragma pack(pop)

typedef struct {
    BMPHeader header;
    unsigned char *data;
} BMPImage;

#define MAGIC_VALUE         0x4D42
#define NUM_PLANE           1
#define COMPRESSION         0
#define NUM_COLORS          0
#define IMPORTANT_COLORS    0
#define BITS_PER_PIXEL      24
#define BITS_PER_BYTE       8

BMPImage *read_bmp(FILE *fp, char **error);

bool write_bmp(FILE *fp, BMPImage *image, char **error);

bool check_bmp_header(BMPHeader *bmp_header, FILE *fp);

void free_bmp(BMPImage *image);

long _get_file_size(FILE *fp);

int _get_image_size_bytes(BMPHeader *bmp_header);

int _get_image_row_size_bytes(BMPHeader *bmp_header);

int _get_bytes_per_pixel(BMPHeader *bmp_header);

int _get_padding(BMPHeader *bmp_header);

bool _check(bool condition, char **error, const char *error_message);

char *_string_duplicate(const char *string);

BMPImage *grey_filter_bmp(BMPImage *image, char **error);

#endif //TUTORIALC_BMP_H
