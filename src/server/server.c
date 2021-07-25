#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MSS_BYTES 575

int server_init(void) {
  int error = 0;
  struct sockaddr_in sa;
  int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket_fd == -1) {
    puts("cannot create socket");
    exit(EXIT_FAILURE);
  }

  int options = getsockopt(socket_fd, TCP_N, int __optname, void *restrict __optval, socklen_t *restrict __optlen)
  memset(&sa, 0, sizeof sa);

  sa.sin_family = AF_INET;
  sa.sin_port = htons(1100);
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

  for (;;) {
    int ConnectFD = accept(socket_fd, NULL, NULL);

    if (ConnectFD == -1) {
      perror("accept failed");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }

    /* perform read write operations ...
    read(ConnectFD, buff, size)
    */

    if (shutdown(ConnectFD, SHUT_RDWR) == -1) {
      perror("shutdown failed");
      close(ConnectFD);
      close(socket_fd);
      exit(EXIT_FAILURE);
    }
    close(ConnectFD);
  }

  close(socket_fd);
  return EXIT_SUCCESS;
}
