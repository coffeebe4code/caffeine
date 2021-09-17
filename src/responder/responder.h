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

typedef struct responder_t {
  char * header;
  char * body;
  int free_body;
  int free_header;
} responder_t;

void responder_init();
void responder_add_default(CODE code, char * header, char * buffer);
void responder_free(responder_t responder);
responder_t responder_get_default(CODE code);
