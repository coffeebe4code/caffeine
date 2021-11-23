#pragma once
#include "../requester/requester.h"
#include "../responder/responder.h"

void barista_add(METHOD method, char *route,
                 void (*func)(requester_t, responder_t *));
void barista_free();
void barista_exec(const int index, const char *buffer, const int buffer_len,
                  responder_t *responder);
