#include "./responder.h"
#include "../debug/debug.h"
#include "stdlib.h"

#define SUPPORTED_CODES 24
responder_t *defaults = NULL;
int responder_len = 0;
int responder_cap = SUPPORTED_CODES;

char *codes1[SUPPORTED_CODES] = {"HTTP/1.1 100 Continue\r\n",
                                 "HTTP/1.1 101 Switching Protocol\r\n",
                                 "HTTP/1.1 200 OK\r\n",
                                 "HTTP/1.1 201 Created\r\n",
                                 "HTTP/1.1 202 Accepted\r\n",
                                 "HTTP/1.1 204 No Content\r\n",
                                 "HTTP/1.1 301 Moved Permanently\r\n",
                                 "HTTP/1.1 302 Found\r\n",
                                 "HTTP/1.1 304 Not Modified\r\n",
                                 "HTTP/1.1 400 Bad Request\r\n",
                                 "HTTP/1.1 401 Unauthorized\r\n",
                                 "HTTP/1.1 403 Forbidden\r\n",
                                 "HTTP/1.1 404 Not Found\r\n",
                                 "HTTP/1.1 405 Method Not Allowed\r\n",
                                 "HTTP/1.1 409 Conflict\r\n",
                                 "HTTP/1.1 410 Gone\r\n",
                                 "HTTP/1.1 413 Entity Too Large\r\n",
                                 "HTTP/1.1 418 Im A Teapot\r\n",
                                 "HTTP/1.1 429 Too Many Requests\r\n",
                                 "HTTP/1.1 500 Internal Server Error\r\n",
                                 "HTTP/1.1 501 Not Implemented\r\n",
                                 "HTTP/1.1 502 Bad Gateway\r\n",
                                 "HTTP/1.1 503 Service Unavailable\r\n",
                                 "HTTP/1.1 504 Gateway Timeout\r\n"};

void responder_init() {
  defaults = malloc(sizeof(responder_t) * responder_cap);
  responder_len = responder_cap;
  for (int i = 0; i < responder_len; i++) {
    responder_add_default((CODE)i, codes1[i], "");
  }
}

void responder_add_default(CODE code, char *header, char *body) {
  responder_t resp = {.header = header,
                      .body = body,
                      .free_body = 0,
                      .free_header = 0,
                      .fidelity = LF};
  defaults[(int)code] = resp;
}

void responder_free(responder_t **responder) {
  if ((*responder)->free_body == 1) {
    free((*responder)->body);
  }
  if ((*responder)->free_header == 1) {
    free((*responder)->header);
  }
}

void responder_get_default(CODE code, responder_t **responder) {
  *responder = &defaults[(int)code];
}
void responder_to_raw(responder_t **resp) {
  responder_t *res = *resp;
  if (res->fidelity == NF) {
  } else if (res->fidelity == LF) {
  } else {
  }
}

char *responder_code_text(CODE code) { return codes1[(int)code]; }

responder_t responder_create_nf(const int free_header,
                                char **buffer,
                                const int len) {
  responder_t res = {.fidelity = NF,
                     .raw_buf = *buffer,
                     .free_body = 0,
                     .free_header = free_header,
                     .raw_len = len};
  return res;
}
