#include "getargs_loop.h"
#include <bits/getopt_core.h>
#include <stdio.h>

int get_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "h")) != -1) {
    switch (opt) {
    case 'h':
      printf("to be implemented\n");
      break;
    default:
      return 0;
    }
  }
  return 1;
}
