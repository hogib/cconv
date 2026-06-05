#include "../include/conv_opts.h"
#include "../include/basic_operators.h"
#include "../include/matrixconv.h"
#include <stdio.h>
#include <stdlib.h>

/* OPERATORS */
static const float sobel_x_h[3] = {-1.0f, 0.0f, 1.0f};
static const float sobel_x_v[3] = {1.0f, 2.0f, 1.0f};

static const float sobel_y_h[3] = {1.0f, 0.0f, -1.0f};
static const float sobel_y_v[3] = {1.0f, 2.0f, 1.0f};

static const float box_3_h[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};
static const float box_3_v[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};

int conv_gaussian_blur(Image *image, float sigma) {
  int k_size = (int)sigma * 6 + 1;
  const float *kernel = create_gaussian_kernel(k_size, sigma);

  if (image->channels_in_file == 4) {
    if ((convolve_separable_rgba(image->data, image->w, image->h, kernel,
                                 kernel, k_size)) != 0)
      return -1;
    else
      return 0;
  } else if (image->channels_in_file == 1) {
    if ((convolve_separable_mono(image->data, image->w, image->h, kernel,
                                 kernel, k_size)) != 0)
      return -1;
    else
      return 0;
  }

  fprintf(stderr, "Provided image needs to be 4 or 2 channels.\n Run [TODO] "
                  "idk either ask user to run some normalize function or do it "
                  "automatically if 3 channel image is provided\n");
  return -2;
}

int conv_sobel_x(Image *image) {
  if (img_grayscale(image) != 0)
    return -2;

  if (convolve_separable_mono(image->data, image->w, image->h, sobel_x_h,
                              sobel_x_v, 3) != 0)
    return -2;

  return 0;
}

int conv_sobel_y(Image *image) {
  if (img_grayscale(image) != 0)
    return -2;

  if (convolve_separable_mono(image->data, image->w, image->h, sobel_y_h,
                              sobel_y_v, 3) != 0)
    return -2;

  return 0;
}

/*FIX: Convolve function clamps results to 8bit.
 * Sobel operator needs results to be unclampted float between passes for
 * accurate results.*/

int conv_sobel(Image *image) {
  if (img_grayscale(image) != 0)
    return -2;

  uint8_t *temp_x = malloc(image->h * image->w * sizeof(size_t(uint8_t)));
  uint8_t *temp_y = malloc(image->h * image->w * sizeof(size_t(uint8_t)));

  free(temp_x);
  free(temp_y);
  return 0;
}
