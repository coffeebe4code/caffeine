#include "./barista.h"
#include <string.h>
#include "stdlib.h"

METHOD * methods;
char * routes;
typedef responder_t * (*FUNCS)(requester_t *, const char *, const int);
FUNCS * execs;
int len = 0;
int cap = 25;

void barista_add(METHOD method, char * route, responder_t (* func)(requester_t, const char *, const int)) {
  if(len == 0) {
    methods = malloc(sizeof(METHOD) * cap);
    routes = malloc(sizeof(const char *) * cap);
    execs = malloc(sizeof((*func)) * cap);
  }
  if(cap == len) {
    cap *= 2;
    methods = realloc(methods, sizeof(METHOD) * cap);
    routes = realloc((void *)routes, sizeof(const char *) * cap);
    execs = realloc(execs, sizeof(FUNCS));
  }
  methods[len] = method;
  strcpy(routes[len], route);

}
