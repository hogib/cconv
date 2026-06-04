#include <stddef.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "basic_operators.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>

/* to be called on main*/
Image load_img(const char *inpath) {
  Image img = {0};
  img.data = stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, 4);

  if (img.data != NULL) {
    img.channels_in_file = 4;
  }
  return img;
}

/* useless? */
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
  if (img == NULL) {
    return;
  }

  stbi_image_free(img->data);
  img->data = NULL;
  img->channels_in_file = 0;
  img->h = 0;
  img->w = 0;
}

/* writes n channel png */
int write_img(const Image *img, const char *outpath) {
  if (!stbi_write_png(outpath, img->w, img->h, img->channels_in_file, img->data,
                      (img->w * img->channels_in_file))) {
    fprintf(stderr, "Image write failed\n");
    return -1;
  }
  return 0;
}

int img_invert_rgba(Image *in_img) {
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;

  if (in_img->channels_in_file == 4) {
    for (size_t i = 0; i < size_pixels * 4u; i += 4) {
      in_img->data[i + 0] = 255u - in_img->data[i + 0]; // R
      in_img->data[i + 1] = 255u - in_img->data[i + 1]; // G
      in_img->data[i + 2] = 255u - in_img->data[i + 2]; // B
      in_img->data[i + 3] = in_img->data[i + 3]; // copy alfa channel over
    }
  } else {
    for (size_t i = 0; i < size_pixels; i++) {
      in_img->data[i] = 255 - in_img->data[i];
    }
  }
  return 0;
}

int img_grayscale(Image *in_img) {
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;
  in_img->channels_in_file = 1;

  for (size_t i = 0, j = 0; i < size_pixels * 4u; i += 4, j += 1) {
    uint8_t gvalue =
        (uint8_t)(in_img->data[i + 0] * 0.299 + in_img->data[i + 1] * 0.587 +
                  in_img->data[i + 2] * 0.114);
    in_img->data[j] = gvalue;
  }
  return 0;
}

int img_binary(Image *in_img) {
  img_grayscale(in_img);
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;

  for (size_t i = 0; i < size_pixels; i++) {
    if (in_img->data[i] > 128u) {
      in_img->data[i] = 0;
    } else {
      in_img->data[i] = 255u;
    }
  }
  return 0;
}
