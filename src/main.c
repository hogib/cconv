#include "../include/basic_operators.h"
#include "../include/cli_options.h"
#include "../include/helpers.h"
#include <stdio.h>
#include <string.h>

typedef enum {
  ACTION_INVERT,
  ACTION_GRAYSCALE,
  ACTION_UNKNOWN,
} ActionIdentifier;

static ActionIdentifier resolve_action(const char *action_string) {
  if (strcmp(action_string, "invert") == 0)
    return ACTION_INVERT;
  if (strcmp(action_string, "grayscale") == 0)
    return ACTION_GRAYSCALE;
  return ACTION_UNKNOWN;
}

int main(int argc, char **argv) {
  cli_action actions = {0};
  get_cli_actions(argc, argv, &actions);
  if (actions.help_called) {
    debug_msg("DEBUG", "Help called, exiting.\n");
    return 0;
  }

  debug_msg("INFO", "Image in path: %s\nImage out path: %s\n", actions.inpath,
            actions.outpath);
  Image image = load_img(actions.inpath);

  for (int i = 0; i < actions.action_count; i++) {
    ActionIdentifier current_action = resolve_action(actions.actions[i]);

    switch (current_action) {
    case ACTION_INVERT:
      debug_msg("INFO", "Current action being performed: %i\n", current_action);
      img_invert_rgba(&image);
      break;

    case ACTION_GRAYSCALE:
      debug_msg("INFO", "Current action being performed: %i\n", current_action);
      img_grayscale(&image);
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
  printf("Destroying image\n");
  destroy_img(&image);

  return 0;
}
