#include "../include/helpers.h"
#include <stdarg.h>
#include <stdio.h>
void debug_msg(const bool state, const char *prefix, const char *format, ...) {
  if (state == false) {
    return;
  }
  fprintf(stderr, "[%s]:\n", prefix);

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}
