#include "../src/server/server.h"
#include <stdio.h>

int main() {
  server_init();
  //server_construct(
  //    HTTP_GET,
  //   "/hello/",
  //   requestor_fn,
  //   responder_fn);
  printf("hello%s\n", " yes");
  server_run();
}
