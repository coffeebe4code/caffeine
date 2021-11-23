#include "../debug/debug.h"
#include "../responder/responder.h"
#include "../utils/utils.h"
#include "./barista.h"
#include "stdlib.h"
#include <string.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

METHOD *bar_methods;
char **routes;
typedef void (*FUNCS)(requester_t, responder_t *);
FUNCS *execs;
int barista_len = 0;
int barista_cap = 25;
const char *barista_error = "Unable to allocate memory in barista process";

void barista_add(METHOD method, char *route,
                 void (*func)(requester_t, responder_t *)) {
  if (barista_len == 0) {
    bar_methods = malloc(sizeof(METHOD) * barista_cap);
    check_pointer_throw(bar_methods, barista_error);
    routes = malloc(sizeof(const char *) * barista_cap);
    check_pointer_throw(routes, barista_error);
    execs = malloc(sizeof(FUNCS) * barista_cap);
    check_pointer_throw(execs, barista_error);
  }
  if (barista_cap == barista_len) {
    barista_cap *= 2;
    bar_methods = realloc(bar_methods, sizeof(METHOD) * barista_cap);
    check_pointer_throw(bar_methods, barista_error);
    routes = realloc((void *)routes, sizeof(const char *) * barista_cap);
    check_pointer_throw(routes, barista_error);
    execs = realloc(execs, sizeof(FUNCS));
    check_pointer_throw(execs, barista_error);
  }
  bar_methods[barista_len] = method;
  char *ptr = malloc(sizeof(char) * strlen(route));
  strcpy(ptr, route);
  routes[barista_len] = ptr;
  execs[barista_len] = func;
  barista_len++;
}

void barista_free() {
  free(bar_methods);
  free(execs);
  for (int i = 0; i < barista_len; i++) {
    free(routes[i]);
  }
  free(routes);
}

void barista_exec(const int index, const char *buffer, const int buffer_len,
                  responder_t *responder) {
  requester_go(index, buffer, buffer_len);
  requester_t header = requester_get(index);
  for (int i = 0; i < barista_len; i++) {
    if (likely(header.method == bar_methods[i])) {
      int tocheck = header.route_end - header.route_start;

      const char *head_buf = header.header;
      int equal = memcmp(&head_buf[header.route_start], routes[i], ++tocheck);

      if (likely(equal == 0)) {
        execs[i](header, responder);
        return;
      }
    }
  }
  responder_get_default(_404_Not_Found, responder);
}
