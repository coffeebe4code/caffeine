#pragma once

typedef struct server_opts {
  int max_conns;
  int max_pending;
} server_opts;

void server_shutdown();
void server_init();
void server_construct();
void server_run();
