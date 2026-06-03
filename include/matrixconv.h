#ifndef MATRIXCONV_H
#define MATRIXCONV_H
#include <stdint.h>
int convolve_separable_rgba(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size);

int convolve_separable_mono(uint8_t *input, int width, int height,
                            const float *kernel_h, const float *kernel_v,
                            int k_size);

#endif
