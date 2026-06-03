#ifndef MATRIXCONV_H
#define MATRIXCONV_H
#include <stdint.h>
int convolve_separable_rgba(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size);

int convolve_separable_mono(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size);

float* create_gaussian_kernel(int k_size, float sigma);

static const float sobel_x_h[3] = {-1.0f, 0.0f, 1.0f};
static const float sobel_x_v[3] = {1.0f, 2.0f, 1.0f};

static const float box_3_h[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};
static const float box_3_v[3] = {1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f};

#endif
