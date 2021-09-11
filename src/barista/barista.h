#pragma once
#include "../header/header.h"
#include "../requester/requester.h"
#include "../responder/responder.h"

void barista_add(METHOD method, char * route, responder_t (* func)(requester_t, const char *, const int));
void barista_free();
responder_t barista_exec(const char * buffer, const int buffer_len);

