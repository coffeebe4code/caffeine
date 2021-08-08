#include "../src/server/server.h"
#include <pthread.h>
#include <unistd.h>

void * test_shutdown() {
  sleep(1);
  server_shutdown();
  return NULL;
}

int main() {
 server_init();
 server_construct();
 
 pthread_t thread_id;
 pthread_create(&thread_id, NULL, &test_shutdown, NULL);
 server_run();
 pthread_join(thread_id, NULL);
 return 0;
}
