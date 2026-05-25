#ifndef GETOPT_H
#define GETOPT_H


typedef struct {
  char *actions[15];
  char *inpath;
  char *outpath;
  int action_count;
} cli_action;

int opt_loop(int argc, char *argv[], cli_action *cli);

#endif
