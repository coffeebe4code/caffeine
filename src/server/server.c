#include "./server.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static struct sockaddr_in sa;
static int socket_fd;

server_t server_init() {
  socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket_fd == -1) {
    puts("cannot create socket");
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
  return 1;
}

server_t server_bind() {
  return 1;
}

server_t server_run() {
  //  for (;;) {
  //    int ConnectFD = accept(socket_fd, NULL, NULL);
  //
  //    if (ConnectFD == -1) {
  //      perror("accept failed");
  //      close(socket_fd);
  //      exit(EXIT_FAILURE);
  //    }
  //
  //    /* perform read write operations ...
  //    read(ConnectFD, buff, size)
  //    */
  //
  //    if (shutdown(ConnectFD, SHUT_RDWR) == -1) {
  //      perror("shutdown failed");
  //      close(ConnectFD);
  //      close(socket_fd);
  //      exit(EXIT_FAILURE);
  //    }
  //    close(ConnectFD);
  //  }
  //
  //  close(socket_fd);
  return 1;
}
