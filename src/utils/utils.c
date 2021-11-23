#include <stdio.h>

void check_pointer_throw(void *ptr, const char *message) {
  if (ptr == NULL) {
    puts(message);
  }
}
