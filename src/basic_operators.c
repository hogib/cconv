#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/basic_operators.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

/* constructor for image. must then be destroyed*/
Image load_img(const char *inpath) {
  Image img = {0};
  img.data =
      stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, 4);
  return img;
}

/* deconstructor that resets image */
static void destroy_img(Image *img) {
  if (img && img->data) {
    img->data = NULL;
    img->channels_in_file = 0;
    img->h = 0;
    img->w = 0;
  }
}


int invert_rgba(const char *inpath, const char *outpath) {
  Image img = load_img(inpath);

  if (!img.data) {
    fprintf(stderr, "stbi_load failed: %s\n", stbi_failure_reason());
    return 2;
  }

  size_t n = (size_t)img.w * (size_t)img.h * 4u;
  for (size_t i = 0; i < n; i += 4) {
    img.data[i + 0] = 255u - img.data[i + 0]; // R
    img.data[i + 1] = 255u - img.data[i + 1]; // G
    img.data[i + 2] =
        255u - img.data[i + 2]; // B
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



/* TODO: Fix function.

// int rgba_binary(const char *inpath, const char *outpath) {
//   grayscale_rgba(inpath, NULL);
//
//   size_t size_pixels = malloc(img.w * img.h);
//}*/
//
//

Image img_grayscale(const Image *in_img) {
  Image out_img = {0};
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;
  out_img.h = in_img->h;
  out_img.w = in_img->w;
  out_img.channels_in_file = 1;
  out_img.data = malloc(size_pixels);

  for (size_t i = 0, j = 0; i < size_pixels * 4u; i += 4, j += 1) {

    uint8_t gvalue =
        (uint8_t)(in_img->data[i + 0] * 0.299 + in_img->data[i + 1] * 0.587 +
                  in_img->data[i + 2] * 0.114);
    out_img.data[j] = gvalue;
  }

  return out_img;
}

int write_img(Image *img, const char *outpath) {
  if (!stbi_write_png(outpath, img->w, img->h, img->channels_in_file, img->data,
                      (img->w * img->channels_in_file))) {
    fprintf(stderr, "Image write failed: %s\n", stbi_failure_reason());
    destroy_img(img);
    return 1;
  }
  destroy_img(img);
  return 0;
}
