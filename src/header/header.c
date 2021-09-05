#include "./header.h"
#include "../debug/debug.h"
#include <string.h>
#ifdef __SIMD__
#include <immintrin.h>
#include <nmmintrin.h>
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

enum METHOD * methods;
int * route_starts;
int * route_ends;
char ** headers;
int * lengths;

void header_reset(const int index) {
  methods[index] = UNSUPPORTED;
  route_starts[index] = 0;
  route_ends[index] = 0;
  lengths[index] = 0;
  free(headers[index]);
}

header_t header_go(int index, const char * buffer, const int buffer_len) {
  header_t header;
  header.index = index;
  lengths[index] = buffer_len;
  headers[index] = buffer;
  return header;  
}

void headers_init(const int total_possible) {
  methods = malloc(sizeof(enum METHOD) * total_possible);
  route_starts = malloc(sizeof(int) * total_possible);
  route_ends = malloc(sizeof(int) * total_possible);
  lengths = malloc(sizeof(int) * total_possible);
  headers = malloc(sizeof(char *) * total_possible);
}

const char get[16] __attribute__((aligned(16))) = "GET ";
const char put[16] __attribute__((aligned(16))) = "PUT ";
const char post[16] __attribute__((aligned(16))) = "POST";
const char del[16] __attribute__((aligned(16))) = "DELE";
const char opt[16] __attribute__((aligned(16))) = "OPTI";
const char pat[16] __attribute__((aligned(16))) = "PATC";
const char spaces[17] __attribute__((aligned(16))) = "                \0";

void parse_route_slow(int index) {
  const char *start_buf = *&(headers[index]) + route_starts[index];
  char *end = strchr(start_buf, ' ');
  if (end != NULL) {
    route_ends[index] = (int)(end - headers[index]) - 1;
  } else {
    methods[index] = UNSUPPORTED;
  }
};

void parse_method_slow(int index, const char *buffer, const int buffer_len) {
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
                methods[index] = UNSUPPORTED;
              } else {
                methods[index] = OPTIONS;
                route_starts[index] = 8;
              }
            } else {
              methods[index] = DELETE;
              route_starts[index] = 7;
            }
          } else {
            methods[index] = PATCH;
            route_starts[index] = 6;
          }
        } else {
          methods[index] = PUT;
          route_starts[index] = 4;
        }
      } else {
        methods[index] = POST;
        route_starts[index] = 5;
      }
    } else {
      methods[index] = GET;
      route_starts[index] = 4;
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

void parse_method_simd(const int index) {
  if (lengths[index] >= 16) {
    register __m128i xmm0;
    xmm0 = _mm_loadu_si128((const __m128i *)headers[index]);
    int equal = cmp((const __m128i *)get, xmm0);

    if (unlikely(!equal)) {
      equal = cmp((const __m128i *)post, xmm0);
      if (unlikely(!equal)) {
        equal = cmp((const __m128i *)put, xmm0);
        if (unlikely(!equal)) {
          equal = cmp((const __m128i *)pat, xmm0);
          if (unlikely(!equal)) {
            equal = cmp((const __m128i *)del, xmm0);
            if (unlikely(!equal)) {
              equal = cmp((const __m128i *)opt, xmm0);
              if (unlikely(!equal)) {
                methods[index] = UNSUPPORTED;
              } else {
                methods[index] = OPTIONS;
                route_starts[index] = 8;
              }
            } else {
              methods[index] = DELETE;
              route_starts[index] = 7;
            }
          } else {
            methods[index] = PATCH;
            route_starts[index] = 6;
          }
        } else {
          methods[index] = PUT;
          route_starts[index] = 4;
        }
      } else {
        methods[index] = POST;
        route_starts[index] = 5;
      }
    } else {
      methods[index] = GET;
      route_starts[index] = 4;
    }
  }
}

void parse_route_simd(const int index, const char *buffer, const int buffer_len) {
  int index_simd;
  int curr_index = route_starts[index];
  register __m128i xmm0, xmm1, xmm2;
  register unsigned int eax;
  register unsigned char ebx;
  while (buffer_len - curr_index >= 16) {
    xmm0 = _mm_loadu_si128((const __m128i *)(buffer + curr_index));
    xmm1 = _mm_loadu_si128((const __m128i *)spaces);
    xmm2 = _mm_cmpeq_epi8(xmm0, xmm1);
    eax = _mm_movemask_epi8(xmm2);
    index_simd = __builtin_ffsll(eax);
    if (index_simd) {
      route_ends[index] = curr_index + index_simd - 2;
      break;
    }
    curr_index += 16;
  }
  if (buffer_len - curr_index > 0 && route_ends[index] == 0) {
    char *end = strchr(buffer + curr_index, ' ');
    if (end != NULL) {
      route_ends[index] = (int)(end - buffer) - 1;
    } else {
      methods[index] = UNSUPPORTED;
    }
  }
}
#endif
void parse_method(const int index) {
#ifdef __SIMD__
  parse_method_simd(index);
#else
  parse_method_slow(index);
#endif
}

void parse_route(const int index) {

#ifdef __SIMD__
  parse_route_slow(index);
#else
  parse_route_slow(index);
#endif
}

