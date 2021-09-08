#include "./barista.h"
#include "stdlib.h"

METHOD * methods;
const char * routes;
responder_t * (*func)(requester_t, const char *, const int);
int len = 0;
int cap = 25;

void barista_add(METHOD method, const char * route, responder_t (* func)(requester_t, const char *, const int)) {
  if(len == 0) {
    methods = malloc(sizeof(METHOD) * cap);
    routes = malloc(sizeof(const char *) * cap);

  }
  if(cap == len) {
    cap *= 2;

  }
}
