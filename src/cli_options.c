#define _GNU_SOURCE
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

/* TODO: define enum for filters */

/* refer to man getopt for global variables */
int opt_loop(int argc, char *argv[]) {
  int ch;
  char *inpath = NULL;
  char *outpath = NULL;

  opterr = 0;

  while ((ch = getopt_long(argc, argv, short_option, long_options, NULL)) !=
         -1) {
    switch (ch) {
    case 'h':
      printf("Usage: \n");
      break;
    case 'i':
      inpath = optarg;
      printf("%s\n", inpath);
      break;
    case 'o':
      outpath = optarg;
      break;
    case 'f':
    /* TODO: with enums */
    default:
      printf("TODO\n");
    }
    return 0;
  }

  return 1;
}
