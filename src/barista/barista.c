#include "./barista.h"
#include "stdlib.h"
#include <string.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

METHOD *methods;
char **routes;
typedef responder_t (*FUNCS)(header_t);
FUNCS *execs;
int len = 0;
int cap = 25;

void barista_add(METHOD method, char *route,
                 responder_t (*func)(header_t)) {
  if (len == 0) {
    methods = malloc(sizeof(METHOD) * cap);
    routes = malloc(sizeof(const char *) * cap);
    execs = malloc(sizeof((*func)) * cap);
  }
  if (cap == len) {
    cap *= 2;
    methods = realloc(methods, sizeof(METHOD) * cap);
    routes = realloc((void *)routes, sizeof(const char *) * cap);
    execs = realloc(execs, sizeof(FUNCS));
  }
  methods[len] = method;
  char *ptr = malloc(sizeof(char) * strlen(route));
  strcpy(ptr, route);
  routes[len] = ptr;
  execs[len] = func;
  len++;
}

void barista_free() {
  free(methods);
  free(execs);
  for (int i = 0; i < len; i++) {
    free(routes[i]);
  }
  free(routes);
}

responder_t barista_exec(const int index, const char *buffer,
                         const int buffer_len) {
  header_go(index, buffer, buffer_len);
  header_t header = header_get(index);
  for (int i = 0; i < len; i++) {
    if (likely(header.method == methods[i])) {
      int tocheck = header.route_end - header.route_start;
      const char *head_buf = header.header;
      int equal = memcmp(&head_buf[header.route_start], routes[i], tocheck);
      if (likely(equal)) {
        return execs[i](header);
      }
    }
  }
  return NULL;
}
