#ifndef CONV_OPTS_H
#define CONV_OPTS_H

#include "point_opts.h"

int conv_gaussian_blur(image_t *image, float sigma);

int conv_sobel_x(image_t *image);
int conv_sobel_y(image_t *image);
int conv_sobel_joint(image_t *image);
int conv_LoG_rgba(image_t *image, float sigma);
#endif
