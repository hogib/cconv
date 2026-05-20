#ifndef BASIC_OPERATORS_H
#define BASIC_OPERATORS_H

#include <stdint.h>


int invert_rgba(const char *inpath, const char *outpath);
void rgba_to_bin(uint8_t *pixels, int w, int h);

#endif
