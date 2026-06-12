#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/point_opts.h"
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

/* to be called on main*/
image_t load_img(const char *inpath) {
  image_t img = {0};
  img.data = stbi_load(inpath, &img.w, &img.h, &img.channels_in_file, 4);

  if (img.data != NULL) {
    img.channels_in_file = 4;
  }
  return img;
}

/* useless? */
static image_t create_img(int width, int height, int channels) {
  image_t img = {0};
  img.w = width;
  img.h = height;
  img.channels_in_file = channels;
  size_t size_bytes = (size_t)width * (size_t)height * channels;
  img.data = malloc(size_bytes);

  return img;
}

/* deconstructor that resets image. also to be called on main */
void destroy_img(image_t *img) {
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
int write_img(const image_t *img, const char *outpath) {
  if (!stbi_write_png(outpath, img->w, img->h, img->channels_in_file, img->data,
                      (img->w * img->channels_in_file))) {
    fprintf(stderr, "Image write failed\n");
    return -1;
  }
  return 0;
}

int img_invert_rgba(image_t *in_img) {
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
      in_img->data[i] = 255u - in_img->data[i];
    }
  }
  return 0;
}

int img_grayscale(image_t *in_img) {
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

int img_binary(image_t *in_img) {
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

int img_binary_threshold(image_t *in_img, uint8_t threshold) {
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;

  for (size_t i = 0; i < size_pixels; i++) {
    if (in_img->data[i] > threshold) {
      in_img->data[i] = 255u;
    } else {
      in_img->data[i] = 0u;
    }
  }
  return 0;
}

int img_contrast_stretch_g(image_t *in_img) {
  size_t size_pixels = (size_t)in_img->w * (size_t)in_img->h;
  uint8_t r_min = 255u;
  uint8_t r_max = 0u;

  for (size_t i = 0; i < size_pixels; ++i) {
    if (in_img->data[i] < r_min)
      r_min = in_img->data[i];
    if (in_img->data[i] > r_max)
      r_max = in_img->data[i];
  }

  if (r_min == r_max)
    r_max = r_min + 1;

  float scale = 255 / (r_max - r_min);

  for (size_t i = 0; i < size_pixels; ++i) {
    float normalized = (in_img->data[i] - r_min) * scale;

    if (normalized > 255u)
      normalized = 255u;
    if (normalized < 0u)
      normalized = 0u;

    in_img->data[i] = (uint8_t)normalized;
  }

  return 0;
}

int img_histogram_eq_g(image_t *in_img) {
  int size_pixels = in_img->w * in_img->h;
  int hist[256] = {0};
  int cdf[256] = {0};
  int eq_map[256] = {0};

  for (int i = 0; i < size_pixels; ++i) {
    hist[in_img->data[i]]++;
  }

  cdf[0] = hist[0];
  for (int i = 1; i < 256; ++i) {
    // Formula for Cumulative Distribution Function
    cdf[i] = cdf[i - 1] + hist[i];
  }

  int cdf_min = 0;
  for (int i = 0; i < 256; i++) {
    if (hist[i] > 0) {
      cdf_min = cdf[i];
      break;
    }
  }

  for (int i = 0; i < 256; i++) {
    if (cdf[i] == 0) {
      eq_map[i] = 0;
    } else {
      // Formula: round( (cdf[i] - cdf_min) / (total_pixels - cdf_min) * 255 )
      eq_map[i] =
          (int)((float)(cdf[i] - cdf_min) / (size_pixels - cdf_min) * 255.0);

      // Clamp mapping
      if (eq_map[i] > 255)
        eq_map[i] = 255;
      if (eq_map[i] < 0)
        eq_map[i] = 0;
    }
  }

  for (int i = 0; i < size_pixels; ++i) {
    in_img->data[i] = (uint8_t)eq_map[in_img->data[i]];
  }

  return 0;
}
