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

void parse_with_simd(const char *buffer, const int buffer_len);
