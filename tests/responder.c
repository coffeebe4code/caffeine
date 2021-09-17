#include "../src/responder/responder.h"
#include <assert.h>

void test_defaults();
void test_responder();

int main() {
  test_defaults();
  test_responder();
  return 0;
}

void test_defaults() {
  responder_init();
  
  responder_t res = responder_get_default(_200_OK);
  assert(res.header == "HTTP/1.1 200 OK\r\n");

  responder_free(res);
  assert(res.header == "HTTP/1.1 200 OK\r\n");
  assert(res.body == "");
  responder_add_default(_200_OK, "server = caffeine", "");
}
