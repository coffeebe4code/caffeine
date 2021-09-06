#include "../src/debug/debug.h"
#include "../src/header/header.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void test_methods();
void test_routes();

int main() {
  headers_init(1);
  test_methods();
  test_routes();
}

void test_routes() {
  header_reset(0);
  header_go(0, "GET / HTTP/1.1\r\n", 16);
  header_t val = header_get(0);
  assert(val.route_end == 4);

  header_reset(0);
  header_go(0, "GET /0000_0000_/ HTTP/1.1\r\n", 27);
  val = header_get(0);
  assert(val.route_end == 15);

  header_reset(0);
  header_go(0, "GET /this/is/a/test/to/the/end", 32);
  val = header_get(0);
  assert(val.method == UNSUPPORTED);

  header_reset(0);
  header_go(
      0, "GET /this/is/a/long/route/designed?to=test&speed=null HTTP/1.1\r\n",
      64444);
  val = header_get(0);
  assert(val.route_end == 52);
}

void test_methods() {
  header_reset(0);
  header_go(0, "GET / HTTP/1.1\r\n", 16);
  header_t val = header_get(0);
  assert(val.method == GET);

  header_go(0, "POST / HTTP/1.1\r\n", 17);
  val = header_get(0);
  assert(val.method == POST);
  
  header_go(0, "PUT / HTTP/1.1\r\n", 16);
  val = header_get(0);
  assert(val.method == PUT);
  
  header_go(0, "OPTIONS / HTTP/1.1\r\n", 20);
  val = header_get(0);
  assert(val.method == OPTIONS);
  
  header_go(0, "PATCH / HTTP/1.1\r\n", 18);
  val = header_get(0);
  assert(val.method == PATCH);
  
  header_go(0, "DELETE / HTTP/1.1\r\n", 19);
  val = header_get(0);
  assert(val.method == DELETE);
  
  header_go(0, "UNSUP / HTTP/1.1\r\n", 18);
  val = header_get(0);
  assert(val.method == UNSUPPORTED);

}
