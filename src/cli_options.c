#define _GNU_SOURCE
#include "../include/cli_options.h"
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

static char *short_option = ":hi:o:f:";
static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"filter", required_argument, NULL, 'f'},
};

/*TODO: decide if I wanna have a help function at all
 * pacman has a help function and I want a help function to maybe display
 * available effects*/
int help(void) {
  printf("Usage: \n");
  return 0;
}

int get_cli_actions(int argc, char *argv[], cli_action *cli) {
  int ch;
  opterr = 0;
  cli->action_count = 0;

  while ((ch = getopt_long(argc, argv, short_option, long_options, NULL)) != -1) {

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

    case 'f':
      if (cli->action_count < 15) {
        cli->actions[cli->action_count++] = optarg;
      } else {
        fprintf(stderr, "Error: Maximum of 15 actions allowwed.\n");
      }

      while (optind < argc && argv[optind][0] != '-') {
        if (cli->action_count < 15) {
          cli->actions[cli->action_count++] = argv[optind];
          optind++;
        } else {
          fprintf(stderr, "Error: Maximum of 15 actions allowed. \n");
          return -1;
        }
      }
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
