// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

// Checks x and y coordinates to determine whether they are in bounds in the specified image.
int32_t in_bounds(struct Image *img, int32_t x, int32_t y); 

// Computes the index of a pixel in an image’s data array given its x and y coordinates
//
// Parameters:
//   img     - pointer to struct Image
//   x       - x coordinate
//   y       - y coordinate
uint32_t compute_index(struct Image *img, int32_t x, int32_t y) {
  return (y * img->width) + x;
}

// Constrains a value so that it is greater than or equal to min and less than or equal to max.
// 
// Parameters:
//   img     - pointer to struct Image
//   min     - minimum value
//   max     - maximum value
int32_t clamp(int32_t val, int32_t min, int32_t max) {
  if (val > max) {
    return max;
  } else if (val < min){
    return min;
  } else {
    return val;
  }
}

// Returns the red component of a pixel color value
// Bits 24-31 are the 8 bit red component value
// 
// Parameters:
//   color     - uint32_t color value
uint8_t get_r(uint32_t color) {
  return (color >> 24) & 255;
}

// Returns the green component of a pixel color value
// Bits 16-23 are the 8 bit green component value
// 
// Parameters:
//   color     - uint32_t color value
uint8_t get_g(uint32_t color) {
  return (color >> 16) & 255;
}

// Returns the blue component of a pixel color value
// Bits 8–15 are the 8 bit blue component value
// 
// Parameters:
//   color     - uint32_t color value
uint8_t get_b(uint32_t color) {
  return (color >> 8) & 255;
}

// Returns the alpha component of a pixel color value
// Bits 0–7 are the 8 bit alpha value
// 
// Parameters:
//   color     - uint32_t color value
uint8_t get_a(uint32_t color) {
  return color & 255; // 0b11111111
}

// Blends foreground and background color component values using a specified alpha (opacity) value.
uint8_t blend_components(uint32_t fg, uint32_t bg, uint32_t alpha);

// Blends foreground and background colors using the foreground color’s alpha value to produce an opaque color
uint32_t blend_colors(uint32_t fg, uint32_t bg);

// Draws a single pixel to a destination image, blending the specified foregroudn color with the existing background color,
// at a specified pixel index
void set_pixel(struct Image *img, uint32_t index, uint32_t color);

// Returns the result of squaring an int64_t value.
int64_t square(int64_t x);

// Returns the sum of the squares of the x and y distance between two points
int64_t square_dist(int64_t x1, int64_t y1, int64_t x2, int64_t y2);

////////////////////////////////////////////////////////////////////////
// API functions
////////////////////////////////////////////////////////////////////////

//
// Draw a pixel.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//   color - uint32_t color value
//
void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) {
  // TODO: implement
}

//
// Draw a rectangle.
// The rectangle has rect->x,rect->y as its upper left corner,
// is rect->width pixels wide, and rect->height pixels high.
//
// Parameters:
//   img     - pointer to struct Image
//   rect    - pointer to struct Rect
//   color   - uint32_t color value
//
void draw_rect(struct Image *img,
               const struct Rect *rect,
               uint32_t color) {
  // TODO: implement
}

//
// Draw a circle.
// The circle has x,y as its center and has r as its radius.
//
// Parameters:
//   img     - pointer to struct Image
//   x       - x coordinate of circle's center
//   y       - y coordinate of circle's center
//   r       - radius of circle
//   color   - uint32_t color value
//
void draw_circle(struct Image *img,
                 int32_t x, int32_t y, int32_t r,
                 uint32_t color) {
  // TODO: implement
}

//
// Draw a tile by copying all pixels in the region
// enclosed by the tile parameter in the tilemap image
// to the specified x/y coordinates of the destination image.
// No blending of the tile pixel colors with the background
// colors should be done.
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//   tilemap - pointer to Image (the tilemap)
//   tile    - pointer to Rect (the tile)
//
void draw_tile(struct Image *img,
               int32_t x, int32_t y,
               struct Image *tilemap,
               const struct Rect *tile) {
 // TODO: implement
}

//
// Draw a sprite by copying all pixels in the region
// enclosed by the sprite parameter in the spritemap image
// to the specified x/y coordinates of the destination image.
// The alpha values of the sprite pixels should be used to
// blend the sprite pixel colors with the background
// pixel colors.
//
// Parameters:
//   img       - pointer to Image (dest image)
//   x         - x coordinate of location where sprite should be copied
//   y         - y coordinate of location where sprite should be copied
//   spritemap - pointer to Image (the spritemap)
//   sprite    - pointer to Rect (the sprite)
//
void draw_sprite(struct Image *img,
                 int32_t x, int32_t y,
                 struct Image *spritemap,
                 const struct Rect *sprite) {
  // TODO: implement
}
