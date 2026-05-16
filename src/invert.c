#define INVERT
#include "invert.h"
static void invert_rgba(uint8_t *pixels, int w, int h) {
  // pixels is w*h*4 bytes, RGBA 8-bit
  size_t n = (size_t)w * (size_t)h * 4u;
  for (size_t i = 0; i < n; i += 4) {
    pixels[i + 0] = 255u - pixels[i + 0]; // R
    pixels[i + 1] = 255u - pixels[i + 1]; // G
    pixels[i + 2] = 255u - pixels[i + 2]; // B
                                          // there's supposed to be an alfa
                                          // channel here leave it alone pls!
  }
}
