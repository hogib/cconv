#include "args_loop.h"
#include <stdio.h>
#include <unistd.h>
int get_args(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "h")) != -1) {
    switch (opt) {
    case 'h':
      printf("Usage: <input_image_path> <output_image.png>\n");
      printf("	-h	Show this message again\n");
      return 0;
    case '?':
      printf("TODO\n");
      return 1;
    default:
      return 1;
    }
  }
  return 1;
}


