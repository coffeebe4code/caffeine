#include "./responder.h"
#include "stdlib.h"

#define SUPPORTED_CODES 23
responder_t *defaults;
int len = 0;
int cap = SUPPORTED_CODES;

char * codes1[SUPPORTED_CODES] = {
  "HTTP/1.1 100 Continue\r\n",
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
  "HTTP/1.1 418 Im A Teapot\r\n",
  "HTTP/1.1 429 Too Many Requests\r\n",
  "HTTP/1.1 500 Internal Server Error\r\n",
  "HTTP/1.1 501 Not Implemented\r\n",
  "HTTP/1.1 502 Bad Gateway\r\n",
  "HTTP/1.1 503 Service Unavailable\r\n",
  "HTTP/1.1 504 Gateway Timeout\r\n"
};

void responder_add_default(CODE code, char *header, char *body) {
  if (len == 0) {
    defaults = malloc(sizeof(responder_t) * cap);
    len = cap;
    for(int i = 0; i < len; i++) {
      defaults[i] = NULL;
    }
  }
  responder_t resp = {.header = header,
                      .body = body,
                      .free_body = 0,
                      .free_header = 0};
  defaults[(int)code] = resp;
}

void responder_free(responder_t responder) {
  if(responder.free_body == 1) {
    free(responder.body);
  }
  if(responder.free_header == 1) {
    free(responder.header); 
  }
}

responder_t responder_get_default(CODE code) {
  return defaults[(int)code];
}
