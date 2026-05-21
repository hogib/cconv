#include "../include/basic_operators.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: <input_image_path> <output_image.png>\n");
    return 4;
  }

  const char *in_path = argv[1];
  const char *out_path = argv[2];

  Image img = load_img(in_path);
  Image gray_img = img_invert_rgba(&img);
  write_img(&gray_img, out_path);
}
