#include "../include/basic_operators.h"
#include "../include/cli_options.h"
#include "../include/conv_opts.h"
#include "../include/helpers.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  ACTION_INVERT,
  ACTION_GRAYSCALE,
  ACTION_BINARY,
  ACTION_GAUSS,
  ACTION_SOBELX,
  ACTION_SOBELY,
  ACTION_UNKNOWN,
} ActionIdentifier;

static ActionIdentifier resolve_action(const char *action_string) {
  if (strcmp(action_string, "invert") == 0)
    return ACTION_INVERT;
  if (strcmp(action_string, "grayscale") == 0)
    return ACTION_GRAYSCALE;
  if (strcmp(action_string, "binary") == 0)
    return ACTION_BINARY;
  if (strcmp(action_string, "gauss") == 0)
    return ACTION_GAUSS;
  if (strcmp(action_string, "sobelx") == 0)
    return ACTION_SOBELX;
  if (strcmp(action_string, "sobely") == 0)
    return ACTION_SOBELY;

  return ACTION_UNKNOWN;
}

int main(int argc, char **argv) {
  cli_action actions = {0};
  get_cli_actions(argc, argv, &actions);

  if (actions.help_called) {
    debug_msg(actions.verbose, "DEBUG", "*/Help called, exiting.*/\n");
    return 0;
  }

  if (actions.inpath == NULL) {
    fprintf(stderr, "Please specify input file.\n");
    help();
    return -3;
  }

  if (actions.outpath == NULL) {
    actions.outpath = "out.png";
  }

  debug_msg(actions.verbose, "INFO",
            "/*Image in path: %s\nImage out path: %s*/\n", actions.inpath,
            actions.outpath);

  Image image = load_img(actions.inpath);

  for (int i = 0; i < actions.action_count; i++) {
    ActionIdentifier current_action = resolve_action(actions.actions[i]);

    switch (current_action) {
    case ACTION_INVERT:
      img_invert_rgba(&image);
      break;

    case ACTION_GRAYSCALE:
      img_grayscale(&image);
      break;

    case ACTION_BINARY:
      img_binary(&image);
      break;

    case ACTION_GAUSS:
      conv_gaussian_blur(&image, 5.0f);
      break;

    case ACTION_SOBELX:
      conv_sobel_x(&image);
      break;

    case ACTION_SOBELY:
      conv_sobel_y(&image);
      break;

    case ACTION_UNKNOWN:
      fprintf(stderr, "Unknown action requested\n");
      return -2;
      break;

    default:
      fprintf(stderr, "Unknown action requested\n");
      return -2;
      break;
    }
  }

  printf("Writing image at %s\n", actions.outpath);
  write_img(&image, actions.outpath);
  debug_msg(actions.verbose, "INFO", "/*Destroying image*/\n");
  destroy_img(&image);

  return 0;
}
