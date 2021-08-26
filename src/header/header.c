#include "./header.h"
#include "../debug/debug.h"
#ifdef __SIMD__
#include <nmmintrin.h>
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

enum METHOD method = UNSUPPORTED;
int error = 0;
char route[255];
const char get[16] __attribute__((aligned(16))) = "GET ";
const char put[16] __attribute__((aligned(16))) = "PUT ";
const char post[16] __attribute__((aligned(16))) = "POST";
const char del[16] __attribute__((aligned(16))) = "DELE";
const char opt[16] __attribute__((aligned(16))) = "OPTI";
const char pat[16] __attribute__((aligned(16))) = "PATC";

void parse_method_slow(const char *buffer, const int buffer_len) {

}
#ifdef __SIMD__
static inline int cmp(const __m128i *method, __m128i xmm0) {
  register __m128i xmm1, xmm2;
  register unsigned int eax;

  xmm1 = _mm_load_si128(method);
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
              }
            } else {
              method = PATCH;
            }
          } else {
            method = DELETE;
          }
        } else {
          method = PUT;
        }
      } else {
        method = POST;
      }
    } else {
      method = GET;
    }
  }
}
#endif
enum METHOD parse_method(const char *buffer, const int buffer_len) {
  #ifdef __SIMD__
    parse_method_simd(buffer, buffer_len);
#else
    parse_method_slow(buffer, buffer_en);
#endif
    return method;
}
void header_parse(const char *buffer, const int buffer_len) {
  enum METHOD method = parse_method(buffer, buffer_len);
 
}
