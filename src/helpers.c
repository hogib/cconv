#include "../include/helpers.h"
#include <stdarg.h>
#include <stdio.h>
void debug_msg(const char *prefix, const char *format, ...) {
  fprintf(stderr, "[%s]", prefix);

  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}
