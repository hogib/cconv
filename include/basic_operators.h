#ifndef BASIC_OPERATORS_H
#define BASIC_OPERATORS_H

#include <stdint.h>

typedef struct {
  uint8_t *data;
  int w;
  int h;
  int channels_in_file;
} Image;



Image load_img(const char *inpath);
Image img_invert_rgba(const Image *in_img);
int rgba_binary(const char *inpath, const char *outpath);
Image img_grayscale(const Image *in_img);
int write_img(Image *img, const char *outpath);
#endif
