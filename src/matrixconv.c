#include "../include/matrixconv.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *f_arrarr_to_uint8(const float *f_arr) {
  // TODO:
  return NULL;
}

float *convolve_rgba(const uint8_t *input, int width, int height,
                     const float *kernel, int k_size) {
  int k_half = k_size / 2;
  int num_pixels = width * height * 4;

  float *output = (float *)malloc(num_pixels * sizeof(float));

  /* 2D Image Pass */
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

      /* 2D Kernel Pass */
      for (int ky = -k_half; ky <= k_half; ++ky) {
        for (int kx = -k_half; kx <= k_half; ++kx) {
          int ix = x + kx;
          int iy = y + ky;

          /* Clamp */
          if (ix < 0)
            ix = 0;
          else if (ix >= width)
            ix = width - 1;

          if (iy < 0)
            iy = 0;
          else if (iy >= height)
            iy = height - 1;

          int pixel_idx = (iy * width + ix) * 4;
          int kernel_idx = (ky + k_half) * k_size + (kx + k_half);
          float weight = kernel[kernel_idx];

          sum_r += input[pixel_idx] * weight;
          sum_g += input[pixel_idx + 1] * weight;
          sum_b += input[pixel_idx + 2] * weight;
        }
      }

      int out_idx = (y * width + x) * 4;

      output[out_idx] = sum_r;
      output[out_idx + 1] = sum_g;
      output[out_idx + 2] = sum_b;
      output[out_idx + 3] = (float)input[out_idx + 3];
    }
  }

  return output;
}

float *convolve_separable_rgba(const uint8_t *input, int width, int height,
                               const float *kernel_h, const float *kernel_v,
                               int k_size) {

  int k_half = k_size / 2;
  int num_pixels = width * height * 4;

  float *temp_buff = (float *)malloc(num_pixels * sizeof(float));
  float *output = (float *)malloc(num_pixels * sizeof(float));

  if (temp_buff == NULL || output == NULL)
    return NULL;

  /* HORIZONTAL */
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

      for (int kx = -k_half; kx <= k_half; ++kx) {
        int ix = x + kx;

        /* Clamp edges */
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
      temp_buff[temp_idx] = sum_r;
      temp_buff[temp_idx + 1] = sum_g;
      temp_buff[temp_idx + 2] = sum_b;
      temp_buff[temp_idx + 3] = input[temp_idx + 3]; // alpha
    }
  }

  /* VERTICAL */
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

      for (int ky = -k_half; ky <= k_half; ++ky) {
        int iy = y + ky;

        /* Clamp edges */
        if (iy < 0)
          iy = 0;
        else if (iy >= height)
          iy = height - 1;

        int temp_idx = (iy * width + x) * 4;
        float weight = kernel_v[ky + k_half];

        sum_r += temp_buff[temp_idx] * weight;
        sum_g += temp_buff[temp_idx + 1] * weight;
        sum_b += temp_buff[temp_idx + 2] * weight;
      }

      int out_idx = (y * width + x) * 4;

      /* Clamp results */
      output[out_idx] = sum_r;
      output[out_idx + 1] = sum_g;
      output[out_idx + 2] = sum_b;
      output[out_idx + 3] = (float)temp_buff[out_idx + 3]; // Restore alpha
    }
  }

  free(temp_buff);
  return output;
}

/* For single channel images */
float *convolve_separable_mono(const uint8_t *input, int width, int height,
                               const float *kernel_h, const float *kernel_v,
                               int k_size) {

  int k_half = k_size / 2;
  int num_pixels = width * height;

  float *temp_buff = (float *)malloc(num_pixels * sizeof(float));
  float *output = (float *)malloc(num_pixels * sizeof(float));

  if (temp_buff == NULL || output == NULL)
    return NULL;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_c = 0.0f;
      for (int kx = -k_half; kx <= k_half; ++kx) {
        int ix = x + kx;

        /* Clamp edges */
        if (ix < 0)
          ix = 0;
        else if (ix >= width)
          ix = width - 1;

        int pixel_idx = (y * width + ix) * 4;
        float weight = kernel_h[kx + k_half];

        sum_c += input[pixel_idx] * weight;
      }

      int temp_idx = (y * width + x);
      temp_buff[temp_idx] = sum_c;
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float sum_c = 0.0f;
      for (int ky = -k_half; ky <= k_half; ++ky) {
        int iy = y + ky;

        /* Clamp edges */
        if (iy < 0)
          iy = 0;
        else if (iy >= height)
          iy = height - 1;

        int temp_idx = (iy * width + x);
        float weight = kernel_v[ky + k_half];

        sum_c += temp_buff[temp_idx] * weight;
      }

      int out_idx = (y * width + x) * 4;

      output[out_idx] = sum_c;
    }
  }

  free(temp_buff);
  return output;
}

/* Generates a 1D Gaussian kernel */
float *create_gaussian_kernel(int k_size, float sigma) {
  // Allocate memory for the 1D kernel
  float *kernel = (float *)malloc(k_size * sizeof(float));
  if (kernel == NULL)
    return NULL;

  float sum = 0.0f;
  int k_half = k_size / 2;

  for (int i = 0; i < k_size; i++) {
    float x = (float)(i - k_half);

    // e^(-x^2 / 2*sigma^2)
    kernel[i] = expf(-(x * x) / (2.0f * sigma * sigma));

    sum += kernel[i];
  }

  // Normalize
  for (int i = 0; i < k_size; i++) {
    kernel[i] /= sum;
  }

  return kernel;
}
