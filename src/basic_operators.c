#include <stddef.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/basic_operators.h"
#include <stdio.h>

/* to be called on main*/
Image load_img(const char *inpath) {
  Image img = {0};
  img.data = stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, 4);
  return img;
}

static Image create_img(int width, int height, int channels) {
  Image img = {0};
  img.w = width;
  img.h = height;
  img.channels_in_file = channels;
  size_t size_bytes = (size_t)width * (size_t)height * channels;
  img.data = malloc(size_bytes);

  return img;
}

/* deconstructor that resets image. also to be called on main */
void destroy_img(Image *img) {
  if (img && img->data) {
    free(img->data);
    img->channels_in_file = 0;
    img->h = 0;
    img->w = 0;
    img->data = NULL;
  }
}

Image img_invert_rgba(const Image *in_img) {
  Image out_img = create_img(in_img->w, in_img->h, in_img->channels_in_file);
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;

  for (size_t i = 0; i < size_pixels * 4u; i += 4) {
    out_img.data[i + 0] = 255u - in_img->data[i + 0]; // R
    out_img.data[i + 1] = 255u - in_img->data[i + 1]; // G
    out_img.data[i + 2] = 255u - in_img->data[i + 2]; // B
    out_img.data[i + 3] = in_img->data[i + 3];        // copy alfa channel over
  }

  return out_img;
}

/* TODO: clean up and error handeling. 1 has to be hardcoded */
Image img_grayscale(const Image *in_img) {
  Image out_img = create_img(in_img->w, in_img->h, 1);
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;

  for (size_t i = 0, j = 0; i < size_pixels * 4u; i += 4, j += 1) {

    uint8_t gvalue =
        (uint8_t)(in_img->data[i + 0] * 0.299 + in_img->data[i + 1] * 0.587 +
                  in_img->data[i + 2] * 0.114);
    out_img.data[j] = gvalue;
  }

  return out_img;
}

Image img_binary(const Image *in_img) {
  /* TODO: write function */
  return *in_img;
}

int write_img(Image *img, const char *outpath) {
  if (!stbi_write_png(outpath, img->w, img->h, img->channels_in_file, img->data,
                      (img->w * img->channels_in_file))) {
    fprintf(stdout, "Image write failed: %s\n", stbi_failure_reason());
    destroy_img(img);
    return 1;
  }
  destroy_img(img);
  return 0;
}
