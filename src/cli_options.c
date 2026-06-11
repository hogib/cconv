#include "../include/cli_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int help(void) {
  printf("Usage: cconv [OPTIONS]\n");
  printf("IO options:\n");
  printf("-i --input    Input file path.\n");
  printf("-o --output   Output file name, path. Will generate output.png if "
         "not specified\n");
  printf("\nImage manipulation options:\n");
  printf("-e --effect   Apply specified effects sequentially.\n");
  printf("Avaliable effects:\ngreyscale, invert, binary\n");
  return 0;
}

static void parse_action_param(action_type_t type, const char *param_str,
                               float *out_f, int *out_i) {
  if (param_str == NULL || param_str[0] == '\0') {
    *out_f = 0.0f;
    *out_i = 0;
    return;
  }
  switch (type) {
  case ACTION_LOG:
    *out_f = atof(param_str);
    break;

  case ACTION_BINARY:
    *out_i = atoi(param_str);
    break;

  case ACTION_GAUSS:
    *out_f = atof(param_str);
    break;

  default:
    break;
  }
}

int get_cli_actions(int argc, char *argv[], cli_action *cli) {
  int ch;
  opterr = 0;
  int option_index = 0;

  cli->action_count = 0;
  cli->verbose = false;
  cli->help_called = false;
  cli->inpath = NULL;
  cli->outpath = NULL;

  while ((ch = getopt_long(argc, argv, ":hvi:o:gl:cbIxyseGt:", long_options,
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
      cli->outpath = optarg;
      break;

    case 'g':

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
