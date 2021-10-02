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
  responder_t res;
  responder_get_default(_200_OK, &res);
  
  responder_to_raw(&res);
  assert(strcmp(res.raw_buf,"HTTP/1.1 200 OK\r\n") == 0);

  responder_free(&res);
  
  responder_t res2;
  responder_add_default(_200_OK, "server = caffeine\r\n", "",19,0);
  responder_get_default(_200_OK, &res2);
  responder_to_raw(&res2);

  assert(strcmp(res2.raw_buf,"HTTP/1.1 200 OK\r\nserver = caffeine\r\n") == 0);
  assert(res2.raw_len == 36);
}
