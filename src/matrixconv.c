#include "../include/matrixconv.h"
#include <stdlib.h>

static uint8_t clamp_to_8bit(float value) {
  if (value < 0)
    return 0;
  if (value > 255.0f)
    return 255;
  return value;
}

/**
 * Applies a separable 2D convolution using two 1D passes.
 * * @param input    1D array of original image pixels (RGBA)
 * @param width    Image width in pixels
 * @param height   Image height in pixels
 * @param kernel_h 1D array for the horizontal kernel weights
 * @param kernel_v 1D array for the vertical kernel weights
 * @param k_size   The dimension of the 1D kernels (e.g., 3)
 *
 * Writes final result to input.
 * convolve_separable_mono to be used for single channel images.
 */

int convolve_separable_rgba(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size) {

  int k_half = k_size / 2;
  int num_pixels = width * height * 4;

  float *temp_buffer = (float *)malloc(num_pixels * sizeof(float));
  if (temp_buffer == NULL)
    return -1;

  /* PASS 1: HORIZONTAL (Reads uint8_t -> Writes float) */
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

      for (int kx = -k_half; kx <= k_half; ++kx) {
        int ix = x + kx;

        /* Clamp to left/right edges */
        if (ix < 0)
          ix = 0;
        else if (ix >= width)
          ix = width - 1;

        int pixel_idx = (y * width + ix) * 4;
        float weight = kernel_h[kx + k_half];

        sum_r += input[pixel_idx] * weight;
        sum_g += input[pixel_idx + 1] * weight;
        sum_b += input[pixel_idx + 2] * weight;
      }

      int temp_idx = (y * width + x) * 4;
      temp_buffer[temp_idx] = sum_r;
      temp_buffer[temp_idx + 1] = sum_g;
      temp_buffer[temp_idx + 2] = sum_b;
      temp_buffer[temp_idx + 3] = input[temp_idx + 3]; // alpha
    }
  }

  /* PASS 2: VERTICAL (Reads float -> Writes uint8_t) */
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

      for (int ky = -k_half; ky <= k_half; ++ky) {
        int iy = y + ky;

        /* Clamp to top/bottom edges */
        if (iy < 0)
          iy = 0;
        else if (iy >= height)
          iy = height - 1;

        int temp_idx = (iy * width + x) * 4;
        float weight = kernel_v[ky + k_half];

        sum_r += temp_buffer[temp_idx] * weight;
        sum_g += temp_buffer[temp_idx + 1] * weight;
        sum_b += temp_buffer[temp_idx + 2] * weight;
      }

      int out_idx = (y * width + x) * 4;

      /* Clamp floating point results */
      input[out_idx] = clamp_to_8bit(sum_r);
      input[out_idx + 1] = clamp_to_8bit(sum_g);
      input[out_idx + 2] = clamp_to_8bit(sum_b);
      input[out_idx + 3] =
          (uint8_t)temp_buffer[out_idx + 3]; /* Restore alpha */
    }
  }

  free(temp_buffer);
  return 0;
}

/* For single channel images */
int convolve_separable_mono(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size) {

  int k_half = k_size / 2;
  int num_pixels = width * height;

  float *temp_buffer = (float *)malloc(num_pixels * sizeof(float));
  if (temp_buffer == NULL)
    return -1;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_c = 0.0f;
      for (int kx = -k_half; kx <= k_half; ++kx) {
        int ix = x + kx;

        /* Clamp to left/right edges */
        if (ix < 0)
          ix = 0;
        else if (ix >= width)
          ix = width - 1;

        int pixel_idx = (y * width + ix) * 4;
        float weight = kernel_h[kx + k_half];

        sum_c += input[pixel_idx] * weight;
      }

      int temp_idx = (y * width + x);
      temp_buffer[temp_idx] = sum_c;
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_c = 0.0f;
      for (int ky = -k_half; ky <= k_half; ++ky) {
        int iy = y + ky;

        /* Clamp to top/bottom edges */
        if (iy < 0)
          iy = 0;
        else if (iy >= height)
          iy = height - 1;

        int temp_idx = (iy * width + x);
        float weight = kernel_v[ky + k_half];

        sum_c += temp_buffer[temp_idx] * weight;
      }

      int out_idx = (y * width + x) * 4;

      /* Clamp floating point results */
      input[out_idx] = clamp_to_8bit(sum_c);
    }
  }

  free(temp_buffer);
  return 0;
}
