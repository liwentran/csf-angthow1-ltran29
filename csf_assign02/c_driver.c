// Driver program to read an image description and render it
// as a PNG image. You should not need to change this code.
// (It's just a demonstration of something useful that can be
// done with the drawing functions.)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "image.h"
#include "drawing_funcs.h"

#define NUM_IMAGE_SLOTS 8

void skipws(FILE *in) {
  for (;;) {
    int c = fgetc(in);
    if (c < 0) {
      break;
    }
    if (!isspace(c)) {
      ungetc(c, in);
      break;
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Error: invalid command line arguments\n");
    return 1;
  }

  struct Image canvas = {
    .data = NULL,
    .width = 0,
    .height = 0,
  };

  struct Image loaded_images[NUM_IMAGE_SLOTS] = {{0,0,NULL}};
  uint32_t width, height;
  char cmd;
  struct Rect rect;
  uint32_t color;
  int32_t x, y, r, n;
  char filename[256];

  int error = 0;

  while (!error && scanf(" %c", &cmd) == 1) {
    switch (cmd) {
    case 'S': // "Size", must be the first command
      if (scanf("%u %u", &width, &height) != 2) {
        error = 1;
        fprintf(stderr, "Error: invalid C command\n");
        break;
      }
      if (init_image(&canvas, width, height) != IMG_SUCCESS) {
        error = 1;
        fprintf(stderr, "Error: could not create canvas\n");
      }
      break;

    case 'R': // "Rectangle"
      if (canvas.data == NULL) {
        error = 1;
        fprintf(stderr, "Error: image size must be specified before drawing operations\n");
      } else if (scanf("%d %d %d %d %x", &rect.x, &rect.y, &rect.width, &rect.height, &color) != 5) {
        error = 1;
        fprintf(stderr, "Error: invalid rectangle\n");
      } else {
        draw_rect(&canvas, &rect, color);
      }
      break;

    case 'C': // "Circle"
      if (canvas.data == NULL) {
        error = 1;
        fprintf(stderr, "Error: image size must be specified before drawing operations\n");
      } else if (scanf("%d %d %d %x", &x, &y, &r, &color) != 4) {
        error = 1;
        fprintf(stderr, "Error: invalid circle\n");
      } else {
        draw_circle(&canvas, x, y, r, color);
      }
      break;

    case 'L': // "Load"
      if (scanf("%d", &n) != 1) {
        error = 1;
        fprintf(stderr, "Error: invalid image number\n");
      } else {
        skipws(stdin);
        if (scanf("%255s", filename) != 1) {
          error = 1;
          fprintf(stderr, "Error: error reading image filename\n");
        } else if (n < 0 || n >= NUM_IMAGE_SLOTS || loaded_images[n].data != NULL) {
          error = 1;
          fprintf(stderr, "Error: invalid image number\n");
        } else if (read_image(filename, &loaded_images[n]) != IMG_SUCCESS) {
          error = 1;
          fprintf(stderr, "Error: could not read image\n");
        }
      }
      break;

    case 'T': // "Tile"
      if (scanf("%d %d %d %d %d %d %d", &n, &rect.x, &rect.y, &rect.width, &rect.height, &x, &y) != 7) {
        error = 1;
        fprintf(stderr, "Error: invalid T command\n");
      } else if (n < 0 || n >= NUM_IMAGE_SLOTS || loaded_images[n].data == NULL) {
        error = 1;
        fprintf(stderr, "Error: invalid image number\n");
      } else {
        draw_tile(&canvas, x, y, &loaded_images[n], &rect);
      }
      break;

    case 'P': // "sPrite"
      if (scanf("%d %d %d %d %d %d %d", &n, &rect.x, &rect.y, &rect.width, &rect.height, &x, &y) != 7) {
        error = 1;
        fprintf(stderr, "Error: invalid P command\n");
      } else if (n < 0 || n >= NUM_IMAGE_SLOTS || loaded_images[n].data == NULL) {
        error = 1;
        fprintf(stderr, "Error: invalid image number\n");
      } else {
        draw_sprite(&canvas, x, y, &loaded_images[n], &rect);
      }
      break;

    default:
      fprintf(stderr, "Error: unrecognized command\n");
      error = 1;
    }
  }

  // try to write output file
  if (!error && write_image(argv[1], &canvas) != IMG_SUCCESS) {
    error = 1;
    fprintf(stderr, "Error: could not write image\n");
  }

  free(canvas.data);
  for (int i = 0; i < NUM_IMAGE_SLOTS; i++) {
    free(loaded_images[i].data);
  }

  return (error != 0); // returns 0 IFF there was no error
}
