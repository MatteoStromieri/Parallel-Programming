#include <stdio.h>
#include "pgm.h"

// Function to write matrix data to a PGM file
void writePGM(const char *filename, unsigned char * matrix, int width, int height) {
    FILE *file = fopen(filename, "wb"); // Open file in binary mode
    if (!file) {
        fprintf(stderr, "Unable to open file %s for writing.\n", filename);
        return;
    }

    // Write PGM header
    fprintf(file, "P5\n%d %d\n255\n", width, height); // P5 for binary grayscale, 255 is the max pixel value

    // Write matrix data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fputc(matrix[i*width+j], file); // Write each pixel value
        }
    }

    fclose(file); // Close the file
}
