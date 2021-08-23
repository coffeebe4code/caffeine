#include "../src/header/header.h"

int main() {
  parse_with_simd("GET / HTTP/1.1", 14);
  parse_with_simd("OPTIONS /",9);
}
