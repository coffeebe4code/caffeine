#pragma once
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_TEST
#define debug_print(fmt, ...) \
  do { if (1) fprintf(stderr, fmt, __VA_ARGS__); } while (0);
#else
#define debug_print(fmt, ...) \
  do {} while (0);
#endif
