#include "./header.h"
#include "../debug/debug.h"
#include <string.h>
#ifdef __SIMD__
#include <nmmintrin.h>
#include <immintrin.h>
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

enum METHOD method = UNSUPPORTED;
int error = 0;
int route_start = 0;
int route_end = 0;
const char get[16] __attribute__((aligned(16))) = "GET ";
const char put[16] __attribute__((aligned(16))) = "PUT ";
const char post[16] __attribute__((aligned(16))) = "POST";
const char del[16] __attribute__((aligned(16))) = "DELE";
const char opt[16] __attribute__((aligned(16))) = "OPTI";
const char pat[16] __attribute__((aligned(16))) = "PATC";
const char spaces[16] __attribute__((aligned(16))) = "                ";

void parse_route_slow(const char *buffer, const int buffer_len) {
  const char *start_buf = buffer + route_start;
  char *end = strchr(start_buf, ' ');
  if (end != NULL) {
    route_end = (int)(end - buffer) - 1;
  } else {
    method = UNSUPPORTED;
  }
}

void parse_method_slow(const char *buffer, const int buffer_len) {
  if (buffer_len >= 16) {
    int equal = memcmp(buffer, get, 4);
    if (unlikely(equal != 0)) {
      equal = memcmp(buffer, post, 4);
      if (unlikely(equal != 0)) {
        equal = memcmp(buffer, put, 4);
        if (unlikely(equal != 0)) {
          equal = memcmp(buffer, pat, 4);
          if (unlikely(equal != 0)) {
            equal = memcmp(buffer, del, 4);
            if (unlikely(equal != 0)) {
              equal = memcmp(buffer, opt, 4);
              if (unlikely(equal != 0)) {
                error = 1;
                method = UNSUPPORTED;
              } else {
                method = OPTIONS;
                route_start = 8;
              }
            } else {
              method = DELETE;
              route_start = 7;
            }
          } else {
            method = PATCH;
            route_start = 6;
          }
        } else {
          method = PUT;
          route_start = 4;
        }
      } else {
        method = POST;
        route_start = 5;
      }
    } else {
      method = GET;
      route_start = 4;
    }
  }
}
#ifdef __SIMD__
static inline int cmp(const __m128i *method, __m128i xmm0) {
  register __m128i xmm1, xmm2;
  register unsigned int eax;

  xmm1 = _mm_loadu_si128(method);
  xmm2 = _mm_cmpeq_epi8(xmm0, xmm1);
  eax = _mm_movemask_epi8(xmm2);
  int equal = (eax & 0x0F) == 0x0F;
  return equal;
}

void parse_method_simd(const char *buffer, const int buffer_len) {
  if (buffer_len >= 16) {
    register __m128i xmm0;
    xmm0 = _mm_loadu_si128((const __m128i *)buffer);
    int equal = cmp((const __m128i *)get, xmm0);

    if (unlikely(!equal)) {
      equal = cmp((const __m128i *)post, xmm0);
      if (unlikely(!equal)) {
        equal = cmp((const __m128i *)put, xmm0);
        if (unlikely(!equal)) {
          equal = cmp((const __m128i *)del, xmm0);
          if (unlikely(!equal)) {
            equal = cmp((const __m128i *)pat, xmm0);
            if (unlikely(!equal)) {
              equal = cmp((const __m128i *)opt, xmm0);
              if (unlikely(!equal)) {
                error = 1;
                method = UNSUPPORTED;
              } else {
                method = OPTIONS;
                route_start = 8;
              }
            } else {
              method = DELETE;
              route_start = 7;
            }
          } else {
            method = PATCH;
            route_start = 6;
          }
        } else {
          method = PUT;
          route_start = 4;
        }
      } else {
        method = POST;
        route_start = 5;
      }
    } else {
      method = GET;
      route_start = 4;
    }
  }
}

void parse_route_simd(const char *buffer, const int buffer_len) {
  int length = buffer_len;
  int index_simd;
  int curr_index = route_start;
  register __m128i xmm0, xmm1, xmm2;
  register unsigned int eax;
  register unsigned char ebx;
  xmm0 = _mm_loadu_si128((const __m128i *)spaces);
  while (length - route_start >= 16) {
    xmm1 = _mm_loadu_si128((const __m128i *)buffer + curr_index);
    xmm2 = _mm_cmpeq_epi8(xmm0, xmm1);
    eax = _mm_movemask_epi8(xmm2);
    ebx = _BitScanForward32(&index_simd,&eax);
    length -= 16;
    curr_index += 16;
  }
  if (length - route_start > 0 && route_end == 0) {
    parse_route_slow(buffer + curr_index,length);
  }
}
#endif
enum METHOD parse_method(const char *buffer, const int buffer_len) {
#ifdef __SIMD__
  parse_method_simd(buffer, buffer_len);
#else
  parse_method_slow(buffer, buffer_len);
#endif
  return method;
}

void parse_route(const char *buffer, const int buffer_len) {

#ifdef __SIMD__
  parse_route_simd(buffer, buffer_len);
#else
  parse_route_slow(buffer, buffer_len);
#endif
}

void header_parse(const char *buffer, const int buffer_len) {
  enum METHOD method = parse_method(buffer, buffer_len);
}
