#ifndef MATRIXCONV_H
#define MATRIXCONV_H
#include <stdint.h>

float *convolve_separable_rgba(const uint8_t *input, int width, int height,
                               const float *kernel_h, const float *kernel_v,
                               int k_size);

float *convolve_separable_mono(const uint8_t *input, int width, int height,
                               const float *kernel_h, const float *kernel_v,
                               int k_size);

float *convolve_rgba(const uint8_t *input, int width, int height,
                     const float *kernel, int k_size);

float *create_gaussian_kernel(int k_size, float sigma);

#endif
