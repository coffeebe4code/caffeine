#include "../src/barista/barista.h"
#include "../src/requester/requester.h"
#include "../src/responder/responder.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/debug/debug.h"

void test_all();
void test_func(requester_t req, responder_t **res);

int main() {
  test_all();
  printf("barista success %s\n", "");
}

void test_all() {
  requester_init(1);
  responder_init();
  barista_add(GET, "/", test_func);
  responder_t *res;
  barista_exec(0, "GET / HTTP/1.1\r\n", 16, &res);
  debug_print("raw %s\n",res->raw_buf);
  //assert(strcmp(res->raw_buf, "raw_buffer") == 0);
  barista_free();
}

void test_func(requester_t req, responder_t **res) {
  assert(strcmp(req.header, "GET / HTTP/1.1\r\n") == 0);
  char * buf = malloc(sizeof(char) * 10);
  strcpy(buf, "raw_buffer");
  **res = responder_create_nf(1, &buf, 10);
}
