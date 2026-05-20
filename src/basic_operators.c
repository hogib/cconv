#include <stdlib.h>
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

/* constructor for image. must then be destroyed*/
static blank_img create_img(const char *inpath, uint out_channels) {
  blank_img img = {0};
  img.data =
      stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, out_channels);
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
  blank_img img = create_img(inpath, 4);

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

int gayscale_rgba(const char *inpath, const char *outpath) {

  blank_img img = create_img(inpath, 4);

  if (!img.data) {
    fprintf(stderr, "stbi_load failed: %s\n", stbi_failure_reason());
    return 2;
  }

  size_t size_pixels = (size_t)img.w * (size_t)img.h;
  uint8_t *temp = malloc(size_pixels);

  if (!temp) {
    fprintf(stderr, "Memory allocation failed!\n");
    return 2;
  }

  for (size_t i = 0, j = 0; i < size_pixels * 4u; i += 4, j += 1) {

    uint8_t gvalue =
        (uint8_t)(img.data[i + 0] * 0.299 + img.data[i + 1] * 0.587 +
                  img.data[i + 2] * 0.114);
    temp[j] = gvalue;
  }

  img.data = realloc(temp, size_pixels);
  int stride_in_bytes = img.w * 1;

  if (!stbi_write_png(outpath, img.w, img.h, 1, img.data, stride_in_bytes)) {
    fprintf(stderr, "stbi_write_png failed: %s\n", outpath);
    destroy_img(&img);
    free(temp);
    return 1;
  }

  destroy_img(&img);
  free(temp);
  return 0;
}
