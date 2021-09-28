#pragma once
#include <stdlib.h>

typedef enum METHOD {
  GET,
  POST,
  PUT,
  DELETE,
  OPTIONS,
  PATCH,
  UNSUPPORTED
} METHOD;

typedef struct requester_t {
  const char * header;
  int index;
  int route_start;
  int route_end;
  size_t buffer_len;
  enum METHOD method;
} requester_t;

void requester_init(const int total_possible);
requester_t requester_get(int index);
void requester_reset(const int index);
void requester_go(int index, const char * buffer, const size_t buffer_len);
void parse_route(int index);
void parse_method(int index);
void requester_free();
