#pragma once
#include "../header/header.h"

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
  char * header;
  char * body;
  int free_body;
  int free_header;
  FIDELITY fidelity;
} responder_t;

void responder_init();
void responder_add_default(CODE code, char * header, char * buffer);
void responder_free(const responder_t *responder);
void responder_get_default(CODE code,const responder_t ** responder);
responder_t responder_create_lf(CODE code,const int free_body, const int free_header, const int keep_alive, char * header, char * body);
responder_t responder_create_nf(CODE code,const int free_body, const int free_header, const int keep_alive, char * buffer);

responder_t responder_create_hf(CODE code,const int free_body, const int free_header, const int keep_alive, char * header, char * body);
responder_t responder_lf_add_header(responder_t resp, char * header);
responder_t responder_hf_add_header(responder_t resp, char * header);
