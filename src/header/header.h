#pragma once

typedef enum METHOD {
  GET,
  POST,
  PUT,
  DELETE,
  OPTIONS,
  PATCH,
  UNSUPPORTED
} METHOD;

typedef struct header_t {
  
} header_t;

static int route_start;
static int route_end;

void parse_route(const char *buffer, const int buffer_len);
enum METHOD parse_method(const char *buffer, const int buffer_len);
