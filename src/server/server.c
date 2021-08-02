#include "./server.h"
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

static struct sockaddr_in sa;
static int socket_fd;
static pthread_t server_id;
static int * clients;

void * handle_controller() {
  return NULL;
}

void server_init() {
  signal(SIGTERM, server_shutdown);
  
  socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket_fd == -1) {
    puts("cannot create socket");
    exit(EXIT_FAILURE);
  }
  int flags = fcntl(socket_fd, F_GETFL);

  if (socket_fd == -1) {
    puts("cannot get flags on socket");
    exit(EXIT_FAILURE);
  }
  fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

  if (socket_fd == -1) {
    puts("cannot set to nonblocking socket");
    exit(EXIT_FAILURE);
  }
  memset(&sa, 0, sizeof sa);

  sa.sin_family = AF_INET;
  sa.sin_port = htons(8080);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, (struct sockaddr *)&sa, sizeof sa) == -1) {
    perror("bind failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, 10) == -1) {
    perror("listen failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
}

void server_construct() {}

void * server_loop() {
  for (;;) {
    int connect_fd = accept(socket_fd, NULL, NULL);

    if (connect_fd == -1) {
      if (errno != EWOULDBLOCK) {
        perror("accept failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
      }
    }
    else {
    }
    close(connect_fd);
  }
  pthread_exit(NULL);
}

void server_run() {
  pthread_create(&server_id, NULL, &server_loop, NULL);
  pthread_join(server_id, NULL);
}

void server_shutdown() {
  shutdown(socket_fd, SHUT_WR);
  pthread_cancel(server_id);
  shutdown(socket_fd, SHUT_RD);
  close(socket_fd);
  exit(EXIT_SUCCESS);
}
