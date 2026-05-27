#ifndef GETOPT_H
#define GETOPT_H
#include <stdbool.h>

typedef struct {
  char *actions[15];
  char *inpath;
  char *outpath;
  int action_count;
  bool help_called;
} cli_action;

int get_cli_actions(int argc, char *argv[], cli_action *cli);

#endif
