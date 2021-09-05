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
  int index;
} header_t;

void headers_init(const int total_possible);
void header_reset(const int index);
header_t header_go(int index, const char * buffer, const int buffer_len);
void parse_route(int index);
void parse_method(int index);
