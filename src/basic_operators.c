#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/basic_operators.h"
#include <stdint.h>
#include <stdio.h>

void invert_rgba(const char *inpath, const char *outpath) {
  // pixels is w*h*4 bytes, RGBA 8-bit

  int w = 0, h = 0, channels_in_file = 0;
  uint8_t *img = stbi_load(inpath, &w, &h, &channels_in_file, 4);

  if (!img) {
    fprintf(stderr, "stbi_load failed: %s\n", stbi_failure_reason());
  }

  size_t n = (size_t)w * (size_t)h * 4u;
  for (size_t i = 0; i < n; i += 4) {
    img[i + 0] = 255u - img[i + 0]; // R
    img[i + 1] = 255u - img[i + 1]; // G
    img[i + 2] = 255u - img[i + 2]; // B
                                    // there's supposed to be an alfa
                                    // channel here leave it alone pls!
  }

  int stride_in_bytes = w * 4;

  if (!stbi_write_png(outpath, w, h, 4, img, stride_in_bytes)) {
    fprintf(stderr, "stbi_write_png failed: %s\n", outpath);
    stbi_image_free(img);
    fprintf(stderr, "Wrote %s\n", outpath);

    stbi_image_free(img);
  }
}
