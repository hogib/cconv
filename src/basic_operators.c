#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/basic_operators.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t *data;
  int w;
  int h;
  int channels_in_file;
} blank_img;

static blank_img create_img(const char *inpath) {
  /* constructor for image. must then be destroyed*/
  blank_img img = {0};
  img.data = stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, 4);
  return img;
}

static void destroy_img(blank_img *img) {
  /* deconstructor that resets image */
  if (img && img->data) {
    img->data = NULL;
    img->channels_in_file = 0;
    img->h = 0;
    img->w = 0;
  }
}

int invert_rgba(const char *inpath, const char *outpath) {
  // pixels is w*h*4 bytes, RGBA 8-bit

  blank_img img = create_img(inpath);

  if (!img.data) {
    fprintf(stderr, "stbi_load failed: %s\n", stbi_failure_reason());
    return 2;
  }

  size_t n = (size_t)img.w * (size_t)img.h * 4u;
  for (size_t i = 0; i < n; i += 4) {
    img.data[i + 0] = 255u - img.data[i + 0]; // R
    img.data[i + 1] = 255u - img.data[i + 1]; // G
    img.data[i + 2] = 255u - img.data[i + 2]; // B
                                // there's supposed to be an alfa
                                // channel here leave it alone pls!
  }

  int stride_in_bytes = img.w * 4;

  if (!stbi_write_png(outpath, img.w, img.h, 4, img.data, stride_in_bytes)) {
    fprintf(stderr, "stbi_write_png failed: %s\n", outpath);
    destroy_img(&img);
    return 1;
  }

  fprintf(stderr, "Wrote %s\n", outpath);
  destroy_img(&img);
  return 0;
}

void greyscale_rgba(const char *inpath, const char *outpath) {}
