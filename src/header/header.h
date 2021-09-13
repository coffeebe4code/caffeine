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
  int route_start;
  int route_end;
  const char * header;
  int buffer_len;
  enum METHOD method;
} header_t;

void headers_init(const int total_possible);
header_t header_get(int index);
void header_reset(const int index);
void header_go(int index, const char * buffer, const int buffer_len);
void parse_route(int index);
void parse_method(int index);
void headers_free();
