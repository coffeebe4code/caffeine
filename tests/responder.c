#include "../src/responder/responder.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/debug/debug.h"

void test_defaults();
void test_responder();

int main() {
  test_defaults();
  

  printf("responder success %s\n", "");
}

void test_defaults() {
  responder_init();
  const responder_t * res;
  responder_get_default(_200_OK, &res);
  debug_print("res header %s\n",res->header);
  assert(strcmp(res->header,"HTTP/1.1 200 OK\r\n") == 0);

  responder_free(res);
  assert(strcmp(res->header,"HTTP/1.1 200 OK\r\n"));
  assert(strcmp(res->body, ""));
  responder_add_default(_200_OK, "server = caffeine", "");
}
