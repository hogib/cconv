#include "../include/conv_opts.h"
#include "../include/matrixconv.h"
#include "point_opts.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OPERATORS */
static const float sobel_x_h[3] = {-1.0f, 0.0f, 1.0f};
static const float sobel_x_v[3] = {1.0f, 2.0f, 1.0f};

static const float sobel_y_h[3] = {1.0f, 0.0f, -1.0f};
static const float sobel_y_v[3] = {1.0f, 2.0f, 1.0f};

static const float box_3_h[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};
static const float box_3_v[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};

int conv_gaussian_blur(image_t *image, float sigma) {
  int k_size = (int)sigma * 6 + 1;
  const float *kernel = create_gaussian_kernel(k_size, sigma);
  size_t size = image->h * image->w * image->channels_in_file;

  if (image->channels_in_file == 4) {
    float *output = convolve_separable_rgba(image->data, image->w, image->h,
                                            kernel, kernel, k_size);
    uint8_t *output_clamped = clamp_float_arr(output, size);

    if (output == NULL || output_clamped == NULL) {
      free(output_clamped);
      free(output);
      return -1;
    }

    memcpy(image->data, output_clamped, size);

    free(output_clamped);
    free(output);

    return 0;
  } else if (image->channels_in_file == 2) {
    float *output = convolve_separable_mono(image->data, image->w, image->h,
                                            kernel, kernel, k_size);
    uint8_t *output_clamped = clamp_float_arr(output, size);

    if (output == NULL || output_clamped == NULL) {
      free(output_clamped);
      free(output);
      return -1;
    }

    memcpy(image->data, output_clamped, size);

    free(output_clamped);
    free(output);

    return 0;
  } else {
    fprintf(stderr,
            "Provided image needs to be 4 or 2 channels.\n Run [TODO] "
            "idk either ask user to run some normalize function or do it "
            "automatically if 3 channel image is provided\n");
    return -2;
  }
}

int conv_sobel_x(image_t *image) {
  if (img_grayscale(image) != 0)
    return -2;

  size_t size = image->h * image->w;
  float *output = convolve_separable_mono(image->data, image->w, image->h,
                                          sobel_x_h, sobel_x_v, 3);
  uint8_t *output_clamped = clamp_float_arr(output, size);

  if (output == NULL || output_clamped == NULL) {
    free(output_clamped);
    free(output);
    return -1;
  }

  memcpy(image->data, output_clamped, size);

  free(output_clamped);
  free(output);

  return 0;
}

int conv_sobel_y(image_t *image) {
  if (img_grayscale(image) != 0)
    return -2;

  size_t size = image->h * image->w;
  float *output = convolve_separable_mono(image->data, image->w, image->h,
                                          sobel_y_h, sobel_y_v, 3);
  uint8_t *output_clamped = clamp_float_arr(output, size);

  if (output == NULL || output_clamped == NULL) {
    free(output_clamped);
    free(output);
    return -1;
  }

  memcpy(image->data, output_clamped, size);

  free(output_clamped);
  free(output);

  return 0;
}

int conv_sobel_joint(image_t *image) {
  size_t size = image->h * image->w * image->channels_in_file;
  float *output_x = convolve_separable_mono(image->data, image->w, image->h,
                                            sobel_x_h, sobel_x_v, 3);
  float *output_y = convolve_separable_mono(image->data, image->w, image->h,
                                            sobel_y_h, sobel_y_v, 3);
  float *output_joint = malloc(size * sizeof(float));

  if (output_x == NULL || output_y == NULL || output_joint == NULL) {
    free(output_x);
    free(output_y);
    free(output_joint);
    return -1;
  }

  for (size_t i = 0; i < size; ++i) {
    output_joint[i] =
        sqrtf(output_x[i] * output_x[i]) + sqrtf(output_y[i] * output_y[i]);
  }

  uint8_t *output_clamped = clamp_float_arr(output_joint, size);

  if (output_clamped == NULL) {
    free(output_x);
    free(output_y);
    free(output_joint);
    free(output_clamped);
    return -1;
  }

  memcpy(image->data, output_clamped, size);
  free(output_x);
  free(output_y);
  free(output_joint);
  free(output_clamped);
  return 0;
}

int conv_LoG_rgba(image_t *image, float sigma) {
  int k_size = 2 * ceil(3 * sigma) + 1;
  size_t size = image->h * image->w * image->channels_in_file;

  float *log_kernel = create_log_kernel(k_size, sigma);
  if (log_kernel == NULL) {
    free(log_kernel);
    return -1;
  }

  float *output =
      convolve_rgba(image->data, image->w, image->h, log_kernel, k_size);
  if (output == NULL) {
    free(output);
    free(log_kernel);
    return -1;
  }

  uint8_t *output_clamped = clamp_float_arr(output, size);
  if (output_clamped == NULL) {
    free(output);
    free(output_clamped);
    free(log_kernel);
    return -1;
  }

  memcpy(image->data, output_clamped, size);
  free(output);
  free(output_clamped);
  free(log_kernel);
  return 0;
}
