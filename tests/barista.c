#include "../src/barista/barista.h"
#include "../src/requester/requester.h"
#include "../src/responder/responder.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_all();
void test_func(requester_t req, responder_t **res);

int main() {
  test_all();
  printf("barista success %s\n", "");
}

void test_all() {
  barista_add(GET, "/", test_func);
  responder_t *res;
  barista_exec(0, "GET / HTTP/1.1\r\n", 16, &res);
  assert(strcmp(res->raw_buf, "raw_buffer") == 0);
  barista_free();
}

void test_func(requester_t req, responder_t **res) {
  assert(strcmp(req.header, "GET / HTTP/1.1\r\n") == 0);
  **res = responder_create_nf(_200_OK, 0, 0, 0, "raw_buffer");
}
