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

void parse_with_simd(const char *buffer, const int buffer_len) {
  const char * value = "GET ";
  __m128i u_str = _mm_loadu_si128((const __m128i *)value);
  __m128i loaded = _mm_loadu_si128((const __m128i *)buffer);
  int not_equal = _mm_cmpestrc(loaded, 4, u_str, 4,
                               _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
  debug_print("has value %d\n", not_equal);
  //if (unlikely(!has_value)) {
  //  u_str.str = "POST ";
  //  has_value = _mm_cmpestrc(loaded, 5, u_str.vec, 5,
  //                           _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
  //  if (!has_value) {
  //    u_str.str = "PUT ";
  //    has_value = _mm_cmpestrc(loaded, 4, u_str.vec, 4,
  //                             _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
  //    if (!has_value) {
  //      u_str.str = "OPT";
  //      has_value = _mm_cmpestrc(loaded, 3, u_str.vec, 3,
  //                               _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
  //      if(!has_value) {
  //        error = 1;
  //      }
  //      else {
  //        method = OPTIONS;
  //      }
  //    } else {
  //      method = PUT;
  //    }
  //  } else {
  //    method = POST;
  //  }
  //} else {
  //  method = GET;
  //}
}

void header_parse(const char *buffer, const int buffer_len) {
// parse first line
#ifdef __SIMD__
  parse_with_simd(buffer, buffer_len);
#else
  parse_normal(buffer, buffer_len);
#endif
  // parse the rest
}
