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

enum METHOD parse_method(const char *buffer, const int buffer_len);
