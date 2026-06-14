#include "../include/cli_options.h"
#include "../include/conv_opts.h"
#include "../include/helpers.h"
#include "../include/point_opts.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  cli_action cli = {0};
  if (get_cli_actions(argc, argv, &cli) != 0)
    return -1;

  if (cli.help_called) {
    debug_msg(cli.verbose, "DEBUG", "*/Help called, exiting.*/\n");
    return 0;
  }

  if (cli.inpath == NULL) {
    fprintf(stderr, "Please specify input file.\n");
    help();
    return -1;
  }

  if (cli.outpath == NULL) {
    cli.outpath = "out.png";
  }

  debug_msg(cli.verbose, "INFO", "/*Image in path: %s\nImage out path: %s*/\n",
            cli.inpath, cli.outpath);

  image_t image = load_img(cli.inpath);

  for (int i = 0; i < cli.action_count; i++) {
    action_t actions = cli.actions[i];
    switch (actions.type) {

    case ACTION_INVERT:
      img_invert_rgba(&image);
      break;

    case ACTION_GRAYSCALE:
      img_grayscale(&image);
      break;

    case ACTION_THRESHOLD:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      img_binary_threshold(&image, actions.param_i);
      break;

    case ACTION_BINARY:
      img_binary(&image);
      break;

    case ACTION_CONTRAST:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      img_contrast_stretch_g(&image);
      break;

    case ACTION_EQUALIZE:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      img_histogram_eq_g(&image);
      break;

    case ACTION_GAUSS:
      conv_gaussian_blur(&image, actions.param_f);
      break;

    case ACTION_SOBEL:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      conv_sobel_joint(&image);
      break;

    case ACTION_SOBELX:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      conv_sobel_x(&image);
      break;

    case ACTION_SOBELY:
      if (image.channels_in_file > 2) {
        img_grayscale(&image);
      }
      conv_sobel_y(&image);
      break;

    case ACTION_LOG:
      conv_LoG_rgba(&image, actions.param_f);
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

  printf("Writing image at %s\n", cli.outpath);
  write_img(&image, cli.outpath);
  debug_msg(cli.verbose, "INFO", "/*Destroying image*/\n");
  destroy_img(&image);

  return 0;
}
