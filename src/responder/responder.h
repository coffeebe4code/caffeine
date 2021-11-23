#pragma once
#include "../requester/requester.h"
#include <stdbool.h>

typedef enum CODE {
  _100_Continue = 0,
  _101_Switching_Protocol,
  _200_OK,
  _201_Created,
  _202_Accepted,
  _204_No_Content,
  _301_Moved_Permanently,
  _302_Found,
  _304_Not_Modified,
  _400_Bad_Request,
  _401_Unauthorized,
  _403_Forbidden,
  _404_Not_Found,
  _405_Method_Not_Allowed,
  _409_Conflict,
  _410_Gone,
  _418_Im_A_Teapot,
  _429_Too_Many_Requests,
  _500_Internal_Server_Error,
  _501_Not_Implemented,
  _502_Bad_Gateway,
  _503_Service_Unavailable,
  _504_Gateway_Timeout
} CODE;

typedef enum FIDELITY {
  NF,
  LF,
  HF,
} FIDELITY;

typedef struct responder_t {
  char *raw_buf;
  int raw_len;
  union {
    char *headers;
    char **dkv_header;
  };
  union {
    int header_len;
    int *dkv_header_len;
  };
  char *body;
  size_t body_len;
  CODE code;
  bool free_body;
  bool free_header;
  FIDELITY fidelity;
} responder_t;

void responder_init();
void responder_add_default(CODE code, char *headers, char *body,
                           size_t header_len, size_t body_len);
void responder_free(responder_t *responder);
void responder_get_default(CODE code, responder_t *responder);
responder_t responder_create_lf(CODE code, const int free_body,
                                const int free_header, const int keep_alive,
                                char *header, char *body);
responder_t responder_create_nf(const int free_header, char *buffer,
                                const int len);
responder_t responder_create_hf(CODE code, const int free_body,
                                const int free_header, const int keep_alive,
                                char *header, char *body);
char *responder_code_text(CODE code);
int responder_code_size(CODE code);
responder_t responder_lf_add_header(responder_t *resp, char *header);
responder_t responder_hf_add_header(responder_t *resp, char *header);
void responder_to_raw(responder_t *resp);
void responder_cleanup();
