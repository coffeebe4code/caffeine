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

int route_start;
int route_end;
METHOD method;
void parse_route(const char *buffer, const int buffer_len);
enum METHOD parse_method(const char *buffer, const int buffer_len);
