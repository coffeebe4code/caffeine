#include "../src/header/header.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>

void test_methods();

int main() {
  test_methods();
#ifdef __PERF__
  clock_t start, stop;
  double cpu_time_used;
  start = clock();
  for(int i = 0; i < 1000000; i++) {
    test_methods();
  }
  stop = clock();
  cpu_time_used = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("test_methods took %f seconds\n",cpu_time_used);
#endif
}


void test_methods() {
  METHOD get = parse_method("GET / HTTP/1.1\r\n", 16);
  METHOD put = parse_method("PUT / HTTP/1.1\r\n", 16);
  METHOD opt = parse_method("OPTIONS / HTTP/1.1\r\n", 20);
  METHOD post = parse_method("POST / HTTP/1.1\r\n", 17);
  METHOD pat = parse_method("PATCH / HTTP/1.1\r\n", 18);
  METHOD del = parse_method("DELETE / HTTP/1.1\r\n", 19);
  METHOD unsup = parse_method("UNSUP / HTTP/1.1\r\n", 18);
  
  assert(get == GET);
  assert(put == PUT);
  assert(opt == OPTIONS);
  assert(post == POST);
  assert(pat == PATCH);
  assert(del == DELETE);
  assert(unsup == UNSUPPORTED);
}
