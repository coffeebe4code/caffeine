#include "../src/header/header.h"
#include <assert.h>



int main() {
  parse_with_simd("GET / HTTP/1.1\r\n", 16);
  parse_with_simd("OPTIONS / HTTP/1.1\r\n", 20);
}


void test_get() {
  assert(parse_with_simd(const char *buffer, const int buffer_len)
