#include "../include/cli_options.h"
#include <bits/getopt_core.h>
#include <getopt.h>
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

int get_cli_actions(int argc, char *argv[], cli_action *cli) {
  int ch;
  opterr = 0;

  cli->action_count = 0;
  cli->verbose = false;
  cli->help_called = false;
  cli->inpath = NULL;
  cli->outpath = NULL;

  while ((ch = getopt_long(argc, argv, ":hvi:o:gl:cbIxyseG::t:", long_options,
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
