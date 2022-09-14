#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct Image {
  uint32_t width;
  uint32_t height;
  uint32_t *data;
};

// return values from init_image, read_image, and write_image
#define IMG_SUCCESS              0
#define IMG_ERR_COULD_NOT_OPEN   -1
#define IMG_ERR_NOT_TRUECOLOR    -2
#define IMG_ERR_MALLOC_FAILED    -3
#define IMG_ERR_COULD_NOT_WRITE  -4

// Initialize an Image struct instance by creating a pixel
// buffer large enough to accommodate an image of the specified
// dimensions, initialzing all pixels to opaque black,
// and initialzing all of the struct Image field values.
// This function only needs to be called if the program
// needs to create an "empty" image in memory.
//
// Parameters:
//   img - pointer to Image instance to initialize
//   width - image width (number of pixel columns)
//   height - image height (number of pixel rows)
//
// Returns:
//   IMG_SUCCESS if successful, otherwise one of the
//   IMG_ERR_* values
int init_image(struct Image *img, uint32_t width, uint32_t height);

// Read PNG image data from a file and initialize the specified
// Image struct instance.
//
// Parameters:
//   filename - name of PNG file to read
//   img - pointer to Image struct to initialize with the loaded
//         image data
//
// Returns:
//   IMG_SUCCESS if successful, otherwise one of the
//   IMG_ERR_* values
int read_image(const char *filename, struct Image *img);

// Write pixel data from specified Image struct instance to the
// named PNG output file.
//
// Parameters:
//   filename - name of PNG file to write
//   img - pointer to Image struct with the pixel data to write
//         to a PNG file
//
// Returns:
//   IMG_SUCCESS if successful, otherwise one of the
//   IMG_ERR_* values
int write_image(const char *filename, struct Image *img);

#endif
