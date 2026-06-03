#include "../include/basic_operators.h"
#include "../include/cli_options.h"
#include "../include/helpers.h"
#include "../include/matrixconv.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  ACTION_INVERT,
  ACTION_GRAYSCALE,
  ACTION_BINARY,
  ACTION_GAUSS,
  ACTION_UNKNOWN,
} ActionIdentifier;


static const float gauss_3_h[3] = {0.25f, 0.50f, 0.25f};
static const float gauss_3_v[3] = {0.25f, 0.50f, 0.25f};

static ActionIdentifier resolve_action(const char *action_string) {
  if (strcmp(action_string, "invert") == 0)
    return ACTION_INVERT;
  if (strcmp(action_string, "grayscale") == 0)
    return ACTION_GRAYSCALE;
  if (strcmp(action_string, "binary") == 0)
    return ACTION_BINARY;
  if (strcmp(action_string, "gauss") == 0)
    return ACTION_GAUSS;

  return ACTION_UNKNOWN;
}

int main(int argc, char **argv) {
  cli_action actions = {0};
  get_cli_actions(argc, argv, &actions);

  if (actions.help_called) {
    debug_msg(actions.verbose, "DEBUG", "Help called, exiting.\n");
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
      debug_msg(actions.verbose, "INFO", "Current action being performed: %i\n",
                current_action);
      img_invert_rgba(&image);
      break;

    case ACTION_GRAYSCALE:
      debug_msg(actions.verbose, "INFO", "Current action being performed: %i\n",
                current_action);
      img_grayscale(&image);
      break;

    case ACTION_BINARY:
      debug_msg(actions.verbose, "INFO", "Current action being performed: %i\n",
                current_action);
      img_binary(&image);
      break;

    case ACTION_GAUSS:
      // TODO:
      if (image.channels_in_file == 4) {
        convolve_separable_rgba(image.data, image.w, image.h, gauss_3_h,
                                gauss_3_v, 3);
        break;
      } else if (image.channels_in_file == 2) {
        convolve_separable_mono(image.data, image.w, image.h, gauss_3_h,
                                gauss_3_v, 3);
        break;
      }
      return -3;

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
  printf("Destroying image\n");
  destroy_img(&image);

  return 0;
}
