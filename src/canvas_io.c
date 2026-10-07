#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <GL/freeglut.h>
#include "canvas_io.h"

static unsigned char *pixels;
static int saved_width, saved_height;

void canvas_capture(int width, int height) {
    unsigned char *next = malloc((size_t)width * height * 3);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, next);

    free(pixels);
    pixels = next;
    fprintf(stderr, "buffer saved\n");
    saved_width = width;
    saved_height = height;
}

void canvas_restore(int width, int height) {
    glPushAttrib(GL_CURRENT_BIT | GL_PIXEL_MODE_BIT);

    glPixelZoom((GLfloat)width / saved_width, (GLfloat)height / saved_height);
    glRasterPos2i(0, 0);
    glDrawPixels(saved_width, saved_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glPopAttrib();
}

void canvas_free(void) {
    free(pixels);
    pixels = NULL;
    saved_width = saved_height = 0;
}

// covent big-endian to little-endian
static void bmp_u32(unsigned char *dst, uint32_t value) {
    dst[0] = (unsigned char)value;
    dst[1] = (unsigned char)(value >> 8);
    dst[2] = (unsigned char)(value >> 16);
    dst[3] = (unsigned char)(value >> 24);
}

int canvas_save_bmp(const char *filename) {
    // 3*3 + 3 del two lb
    // mod 4 = 0
    uint64_t stride64 = ((uint64_t)saved_width * 3 + 3) & ~(uint64_t)3;
    uint64_t bytes64 = stride64 * (uint64_t)saved_height;
    if (bytes64 > UINT32_MAX - 54 || stride64 > SIZE_MAX) return 0;

    size_t stride = (size_t)stride64;
    unsigned char *row = calloc(1, stride);
    if (!row) return 0;

    FILE *file = fopen(filename, "wbx");
    if (!file) { free(row); return 0; }

    // header
    unsigned char header[54] = {0};
    header[0] = 'B'; header[1] = 'M'; // 'BM'
    bmp_u32(header + 2, (uint32_t)bytes64 + 54); // file size
    bmp_u32(header + 10, 54); // start
    bmp_u32(header + 14, 40); // image header
    bmp_u32(header + 18, (uint32_t)saved_width); // image w
    bmp_u32(header + 22, (uint32_t)saved_height); // h
    header[26] = 1;
    header[28] = 24; // 3byte 1pixel
    bmp_u32(header + 34, (uint32_t)bytes64); //pixel size
    int ok = fwrite(header, 1, sizeof header, file) == sizeof header;

    for (int y = 0; ok && y < saved_height; ++y) {
        const unsigned char *src = pixels + (size_t)y * saved_width * 3;
        // RGB to BGR
        for (int x = 0; x < saved_width; ++x) {
            size_t i = (size_t)x * 3;
            row[i] = src[i + 2];
            row[i + 1] = src[i + 1];
            row[i + 2] = src[i];
        }
        ok = fwrite(row, 1, stride, file) == stride;
    }
    if (fclose(file) != 0) ok = 0;
    free(row);
    if (!ok) remove(filename);
    return ok;
}
