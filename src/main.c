#include "../include/basic_operators.h"
#include "../include/cli_options.h"
#include <stdio.h>

int main(int argc, char **argv) {

  cli_action actions = {0};
  get_cli_actions(argc, argv, &actions);
}
