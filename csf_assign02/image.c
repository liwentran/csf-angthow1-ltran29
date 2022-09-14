#include <stdio.h>
#include <stdlib.h>
#include "pnglite.h"
#include "image.h"

int png_init_called;

int is_little_endian(void) {
  int32_t x = 1;
  return *((char *) &x) == 1;
}

uint32_t byteswap(uint32_t val) {
  const uint8_t *p = (const uint8_t *) &val;
  uint32_t result = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
  return result;
}

int init_image(struct Image *img, uint32_t width, uint32_t height) {
  unsigned num_pixels = width * height;

  uint32_t *pixel_data = (uint32_t *) malloc(num_pixels * sizeof(uint32_t));
  if (pixel_data == NULL) {
    return IMG_ERR_MALLOC_FAILED;
  }

  // initialize every pixel to opaque black
  for (uint32_t i = 0; i < num_pixels; i++) {
    pixel_data[i] = 0x000000FFU;
  }

  // success
  img->width = width;
  img->height = height;
  img->data = pixel_data;
  return IMG_SUCCESS;
}

int read_image(const char *filename, struct Image *img) {
  if (!png_init_called) {
    png_init(0, 0);
    png_init_called = 1;
  }

  png_t png;

  if (png_open_file_read(&png, filename) != PNG_NO_ERROR) {
    return IMG_ERR_COULD_NOT_OPEN;
  }

  // only allow truecolor 8bpp images
  if (!(png.color_type == PNG_TRUECOLOR && png.bpp == 3) &&
      !(png.color_type == PNG_TRUECOLOR_ALPHA && png.bpp == 4)) {
    png_close_file(&png);
    return IMG_ERR_NOT_TRUECOLOR;
  }

  unsigned num_pixels = png.width * png.height;

  // allocate buffer for pixel data in truecolor RGBA format
  uint32_t *pixel_data = (uint32_t *) malloc(num_pixels * sizeof(uint32_t));

  if (png.color_type == PNG_TRUECOLOR) {
    // PNG pixel data is in RGB form, expand it to add the alpha channel

    unsigned char *pixel_data_raw = (unsigned char *) malloc(num_pixels * 3);
    if (png_get_data(&png, pixel_data_raw) != PNG_NO_ERROR) {
      png_close_file(&png);
      free(pixel_data);
      return IMG_ERR_MALLOC_FAILED;
    }

    for (unsigned i = 0; i < num_pixels; i++) {
      unsigned char r = pixel_data_raw[i*3 + 0];
      unsigned char g = pixel_data_raw[i*3 + 1];
      unsigned char b = pixel_data_raw[i*3 + 2];
      unsigned char a = 255;

      pixel_data[i] = (r << 24) | (g << 16) | (b << 8) | a;
    }

    free(pixel_data_raw);
  } else {
    // PNG pixel data is already in the correct format,
    // except that the RGBA data is in big-endian form, so we
    // need to byteswap if on a little endian system
    if (png_get_data(&png, (unsigned char *) pixel_data) != PNG_NO_ERROR) {
      png_close_file(&png);
      free(pixel_data);
      return IMG_ERR_MALLOC_FAILED;
    }

    if (is_little_endian()) {
      for (unsigned i = 0; i < num_pixels; i++) {
        pixel_data[i] = byteswap(pixel_data[i]);
      }
    }
  }

  // communicate pixel data and image dimensions to caller
  img->data = pixel_data;
  img->width = png.width;
  img->height = png.height;

  png_close_file(&png);

  return IMG_SUCCESS;
}

int write_image(const char *filename, struct Image *img) {
  if (!png_init_called) {
    png_init(0, 0);
    png_init_called = 1;
  }

  png_t png;

  if (png_open_file_write(&png, filename) != PNG_NO_ERROR) {
    return IMG_ERR_COULD_NOT_OPEN;
  }

  // if this is a little endian system, we need to byteswap
  // every uint32_t so that it can be written in big-endian order
  // (which is what PNG requires)

  uint32_t *data_to_write = img->data;
  int need_byteswap = is_little_endian();

  if (need_byteswap) {
    data_to_write = (uint32_t *) malloc(img->width * img->height * sizeof(uint32_t));
    if (data_to_write == NULL) {
      png_close_file(&png);
      return IMG_ERR_MALLOC_FAILED;
    }

    uint32_t num_pixels = img->width * img->height;
    for (uint32_t i = 0; i < num_pixels; i++) {
      data_to_write[i] = byteswap(img->data[i]);
    }
  }

  int rc = png_set_data(&png, img->width, img->height, 8, PNG_TRUECOLOR_ALPHA, (unsigned char *) data_to_write);
  int success = (rc == PNG_NO_ERROR);

  png_close_file(&png);
  if (need_byteswap) {
    free(data_to_write);
  }

  return success ? IMG_SUCCESS : IMG_ERR_COULD_NOT_WRITE;
}
