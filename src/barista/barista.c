#include "./barista.h"
#include "../../src/responder/responder.h"
#include "stdlib.h"
#include <string.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

METHOD *methods;
char **routes;
typedef responder_t (*FUNCS)(header_t, responder_t *);
FUNCS *execs;
int barista_len = 0;
int barista_cap = 25;

void barista_add(METHOD method, char *route,
                 responder_t (*func)(header_t,responder_t *)) {
  if (barista_len == 0) {
    methods = malloc(sizeof(METHOD) * barista_cap);
    routes = malloc(sizeof(const char *) * barista_cap);
    execs = malloc(sizeof((*func)) * barista_cap);
  }
  if (barista_cap == barista_len) {
    barista_cap *= 2;
    methods = realloc(methods, sizeof(METHOD) * barista_cap);
    routes = realloc((void *)routes, sizeof(const char *) * barista_cap);
    execs = realloc(execs, sizeof(FUNCS));
  }
  methods[barista_len] = method;
  char *ptr = malloc(sizeof(char) * strlen(route));
  strcpy(ptr, route);
  routes[barista_len] = ptr;
  execs[barista_len] = func;
  barista_len++;
}

void barista_free() {
  free(methods);
  free(execs);
  for (int i = 0; i < barista_len; i++) {
    free(routes[i]);
  }
  free(routes);
}

void barista_exec(const int index, const char *buffer,
                         const int buffer_len, responder_t *responder) {
  header_go(index, buffer, buffer_len);
  header_t header = header_get(index);
  for (int i = 0; i < barista_len; i++) {
    if (likely(header.method == methods[i])) {
      int tocheck = header.route_end - header.route_start;
      const char *head_buf = header.header;
      int equal = memcmp(&head_buf[header.route_start], routes[i], tocheck);
      if (likely(equal)) {
        execs[i](header, responder);
        return;
      }
    }
  }
  responder_get_default(_404_Not_Found, &responder);
}
