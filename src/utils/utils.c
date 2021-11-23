#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void check_pointer_throw(void *ptr, const char *message) {
  if (ptr == NULL) {
    puts(message);
    exit(EXIT_FAILURE);
  }
}
