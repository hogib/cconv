#ifndef POINT_OPTS_H
#define POINT_OPTS_H

#include <stdint.h>

typedef struct {
  uint8_t *data;
  int w;
  int h;
  int channels_in_file;
} image_t;

image_t load_img(const char *inpath);
int img_invert_rgba(image_t *in_img);
int img_grayscale(image_t *in_img);
int img_binary(image_t *in_img);
int write_img(const image_t *img, const char *outpath);
void destroy_img(image_t *img);
int img_contrast_stretch_g(image_t *in_img);
int img_binary_threshold(image_t *in_img, uint8_t threshold);
float *create_log_kernel(int k_size, float sigma);
#endif
