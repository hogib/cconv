#ifndef CONV_OPTS_H
#define CONV_OPTS_H

#include "basic_operators.h"

int conv_gaussian_blur(Image *image, float sigma);

int conv_sobel_x(Image *image);
int conv_sobel_y(Image *image);
#endif
