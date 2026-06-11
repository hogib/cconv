#ifndef GETOPT_H
#define GETOPT_H
#include <getopt.h>
#include <stdbool.h>

#define MAX_ACTIONS 15
#define MAX_ACTION_NAME 32

typedef enum {
  ACTION_INVERT,
  ACTION_GRAYSCALE,
  ACTION_BINARY,
  ACTION_THRESHOLD,
  ACTION_GAUSS,
  ACTION_SOBELX,
  ACTION_SOBELY,
  ACTION_SOBEL,
  ACTION_LOG,
  ACTION_CONTRAST,
  ACTION_EQUALIZE,
  ACTION_UNKNOWN,
} action_type_t;

typedef struct {
  action_type_t type;
  float param_f;
  int param_i;
} action_t;

typedef struct {
  char *inpath;
  char *outpath;
  action_t actions[MAX_ACTIONS];
  int action_count;
  bool help_called;
  bool verbose;
} cli_action;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"verbose", no_argument, 0, 'v'},

    // Effects

    {"grayscale", no_argument, 0, 'g'},
    {"log", required_argument, 0, 'l'},
    {"contrast", no_argument, 0, 'c'},
    {"binary", no_argument, 0, 'b'},
    {"threshold", required_argument, 0, 't'},
    {"invert", no_argument, 0, 'I'},
    {"gauss", optional_argument, 0, 'G'},
    {"sobelx", no_argument, 0, 'x'},
    {"sobely", no_argument, 0, 'y'},
    {"sobel", no_argument, 0, 's'},
    {"equalize", no_argument, 0, 'e'},

};

int get_cli_actions(int argc, char *argv[], cli_action *cli);
int help(void);

#endif
