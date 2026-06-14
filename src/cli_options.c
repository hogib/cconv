#include "../include/cli_options.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_cli_actions(int argc, char *argv[], cli_action *cli) {
  int ch;
  opterr = 0;

  cli->action_count = 0;
  cli->verbose = false;
  cli->help_called = false;
  cli->inpath = NULL;
  cli->outpath = NULL;

  while ((ch = getopt_long(argc, argv, ":hvi:o::gl::cbIxyseG::t:", long_options,
                           NULL)) != -1) {

    switch (ch) {
    case 'h':
      help();
      cli->help_called = true;
      return 0;

    case 'i':
      cli->inpath = optarg;
      break;

    case 'o':
      if (optarg != NULL) {
        cli->outpath = optarg;
      } else {
        cli->outpath = "out.png";
      }
      break;

    case 'g':
      cli->actions[cli->action_count].type = ACTION_GRAYSCALE;
      ++cli->action_count;
      break;

    case 'l':
      cli->actions[cli->action_count].type = ACTION_LOG;
      if (optarg != NULL) {
        cli->actions[cli->action_count].param_f = atof(optarg);
      } else {
        cli->actions[cli->action_count].param_f = 0.55f;
      }
      ++cli->action_count;
      break;

    case 'c':
      cli->actions[cli->action_count].type = ACTION_CONTRAST;
      ++cli->action_count;
      break;

    case 'b':
      cli->actions[cli->action_count].type = ACTION_BINARY;
      ++cli->action_count;
      break;

    case 'I':
      cli->actions[cli->action_count].type = ACTION_INVERT;
      ++cli->action_count;
      break;

    case 'x':
      cli->actions[cli->action_count].type = ACTION_SOBELX;
      ++cli->action_count;
      break;

    case 'y':
      cli->actions[cli->action_count].type = ACTION_SOBELY;
      ++cli->action_count;
      break;

    case 's':
      cli->actions[cli->action_count].type = ACTION_SOBEL;
      ++cli->action_count;
      break;

    case 'e':
      cli->actions[cli->action_count].type = ACTION_EQUALIZE;
      ++cli->action_count;
      break;

    case 'G':
      cli->actions[cli->action_count].type = ACTION_GAUSS;
      if (optarg != NULL) {
        cli->actions[cli->action_count].param_f = atof(optarg);
      } else {
        cli->actions[cli->action_count].param_f = 5.0f;
      }
      ++cli->action_count;
      break;

    case 't':
      cli->actions[cli->action_count].type = ACTION_THRESHOLD;
      if (optarg != NULL) {
        cli->actions[cli->action_count].param_i = atoi(optarg);
      } else {
        fprintf(stderr,
                "Error: Expected argument for threshold. (Expected 1, got 0)");
        return -1;
      }
      ++cli->action_count;
      break;

    case 'v':
      cli->verbose = true;
      break;

    case '?':
      fprintf(stderr, "Error: Unknown option passed.\n");
      return -1;

    default:
      fprintf(stderr, "Error: Unhandled option. \n");
      return -1;
    }
  }
  return 0;
}

int help(void) {
  printf("\n");
  printf("USAGE:\n");
  printf("  cconv [OPTIONS] -i INPUT_FILE [-o OUTPUT_FILE]\n\n");

  printf("REQUIRED OPTIONS:\n");
  printf("  -i, --input FILE          Input image file (PNG/JPG)\n\n");

  printf("OUTPUT OPTIONS:\n");
  printf("  -o, --output FILE         Output file path (default: out.png)\n\n");

  printf("IMAGE FILTERS (apply sequentially):\n");
  printf("  Basic Filters:\n");
  printf("    -g, --grayscale         Convert image to grayscale\n");
  printf("    -I, --invert            Invert image colors (negative)\n");
  printf("    -c, --contrast          Apply contrast stretching (histogram "
         "equalization)\n");
  printf("    -e, --equalize          Histogram equalization\n\n");

  printf("  Threshold & Convolution:\n");
  printf("    -b, --binary            Apply binary thresholding (B&W "
         "conversion)\n");
  printf("    -t, --threshold VAL     Binary threshold value (0-255, default: "
         "128)\n");
  printf("    -l, --log SIGMA         Laplacian of Gaussian filter\n");
  printf("                            (sigma: 0.1-10.0, default: 0.55)\n");
  printf("    -G, --gauss [SIGMA]     Gaussian blur filter\n");
  printf("                            (sigma: 0.1-10.0, default: 5.0)\n\n");

  printf("  Edge Detection:\n");
  printf("    -x, --sobelx            Sobel X edge detection (horizontal)\n");
  printf("    -y, --sobely            Sobel Y edge detection (vertical)\n");
  printf("    -s, --sobel             Sobel edge detection (combined X+Y)\n\n");

  printf("OTHER OPTIONS:\n");
  printf(
      "  -v, --verbose             Enable verbose output (debug messages)\n");
  printf("  -h, --help                Display this help message\n\n");

  printf("EXAMPLES:\n");
  printf("  # Basic grayscale conversion\n");
  printf("  cconv -i photo.jpg -o result.png -g\n\n");

  printf("  # Multiple filters (applied sequentially)\n");
  printf("  cconv -i photo.jpg -o result.png -g -c -b\n\n");

  printf("  # Combined short options\n");
  printf("  cconv -i photo.jpg -o result.png -gcI\n\n");

  printf("  # LoG filter with custom sigma\n");
  printf("  cconv -i photo.jpg -o result.png -l 2.5\n\n");

  printf("  # Edge detection with custom threshold\n");
  printf("  cconv -i photo.jpg -o result.png -x -t 100\n\n");

  printf("  # Verbose mode with multiple filters\n");
  printf("  cconv -i photo.jpg -o result.png -v -g -l 1.5 -s\n\n");

  printf("FILTER ORDER MATTERS:\n");
  printf("  Filters are applied in the order specified on command line.\n");
  printf("  Example: cconv -i in.jpg -o out.png -g -l 1.5 -c\n");
  printf("  This applies: grayscale → LoG → contrast\n\n");

  printf("NOTES:\n");
  printf("  • Optional arguments can be combined: -gIc is same as -g -I -c\n");
  printf("  • Parametered options must be last in combined form: use -gl 1.5 "
         "not -l1.5g\n");
  printf("  • If no -o specified, output defaults to 'out.png'\n");
  printf("  • Use -v flag for detailed processing information\n\n");

  return 0;
}
