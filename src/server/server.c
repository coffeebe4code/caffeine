#include "./server.h"
#include "../debug/debug.h"
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static struct sockaddr_in sa;
static int socket_fd;
static pthread_t *server_ids;
static int max_conns = 990;
static int *client_fds;

void *handle_controller() { return NULL; }

void server_init() {
  signal(SIGTERM, server_shutdown);
  signal(SIGCHLD, SIG_IGN);
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

  if (listen(socket_fd, SOMAXCONN) == -1) {
    perror("listen failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
  client_fds = calloc(max_conns, sizeof(int *));
}

void server_construct() {}

void *server_loop(void *client_id) {
  for (;;) {
    int id = accept(socket_fd, NULL, NULL);
    client_fds[*(int *)client_id] = id;
    if (id == -1) {
      if (errno != EWOULDBLOCK) {
        perror("accept failed");
        close(socket_fd);
        break;
      }
    } else {
      char buf[4096];
      int buf_len = 0;
      while (1) {
        int rd = read(id, buf + buf_len, sizeof(buf));
        if (rd > 0) {
          int wr = write(id,
                         "HTTP/1.1 200 OK\r\nServer: "
                         "caffeine\r\nContent-Length: 5\r\n\r\nHello",
                         71);
          if (wr == -1) {
            perror("write failed");
            break;
          }
        } else if (id == 0) {
          break;
        } else if (errno != EWOULDBLOCK) {
          perror("read failed");
          break;
        }
      }
    }
    id = close(id);
  }
  pthread_exit(NULL);
}

void server_run() {
  server_ids = (pthread_t *)malloc(max_conns * sizeof(pthread_t *));

  for (int i = 0; i < (max_conns); i++) {
    pthread_create(&server_ids[i], NULL, &server_loop, (void *)(&i));
  }
  for (int i = 0; i < (max_conns); i++) {
    pthread_join(server_ids[i], NULL);
  }
}

void server_shutdown() {
  shutdown(socket_fd, SHUT_WR);
  for (int i = 0; i < max_conns; i++) {
    pthread_cancel(server_ids[i]);
    if (client_fds[i] != 0 || client_fds[i] != -1) {
      close(client_fds[i]);
    }
  }
  free(server_ids);
  free(client_fds);
  shutdown(socket_fd, SHUT_RD);
  close(socket_fd);
  exit(EXIT_SUCCESS);
}
