#include "../src/debug/debug.h"
#include "../src/requester/requester.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void test_methods();
void test_routes();

int main() {
  requester_init(1);
  test_methods();
  test_routes();
  requester_free();
  printf("requester success %s\n", "");
}

void test_routes() {
  requester_reset(0);
  requester_go(0, "GET / HTTP/1.1\r\n", 16);
  requester_t val = requester_get(0);
  assert(val.route_end == 4);
  assert(val.route_start == 4);

  requester_reset(0);
  requester_go(0, "GET /0000_0000_/ HTTP/1.1\r\n", 27);
  val = requester_get(0);
  assert(val.route_end == 15);

  requester_reset(0);
  requester_go(0, "GET /this/is/a/test/to/the/end", 32);
  val = requester_get(0);
  assert(val.method == UNSUPPORTED);

  requester_reset(0);
  requester_go(
      0, "GET /this/is/a/long/route/designed?to=test&speed=null HTTP/1.1\r\n",
      64);
  val = requester_get(0);
  assert(val.route_end == 52);
}

void test_methods() {
  requester_reset(0);
  requester_go(0, "GET / HTTP/1.1\r\n", 16);
  requester_t val = requester_get(0);
  assert(val.method == GET);

  requester_go(0, "POST / HTTP/1.1\r\n", 17);
  val = requester_get(0);
  assert(val.method == POST);
  
  requester_go(0, "PUT / HTTP/1.1\r\n", 16);
  val = requester_get(0);
  assert(val.method == PUT);
  
  requester_go(0, "OPTIONS / HTTP/1.1\r\n", 20);
  val = requester_get(0);
  assert(val.method == OPTIONS);
  
  requester_go(0, "PATCH / HTTP/1.1\r\n", 18);
  val = requester_get(0);
  assert(val.method == PATCH);
  
  requester_go(0, "DELETE / HTTP/1.1\r\n", 19);
  val = requester_get(0);
  assert(val.method == DELETE);
  
  requester_go(0, "UNSUP / HTTP/1.1\r\n", 18);
  val = requester_get(0);
  assert(val.method == UNSUPPORTED);

}
