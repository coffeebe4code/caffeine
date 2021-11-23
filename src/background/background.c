#include "../utils/utils.h"
#include "./background.h"
#include "time.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_t bg_thread;
static pthread_t *proc_threads;
typedef void *(*BG_FUNC)(void *);
BG_FUNC *bg_funcs;
void **raw_datas;
clock_t *starts;
int *successes;
long *delays;
int *kills;
int bg_len = 0;
int bg_cap = 25;
static clock_t last_ran;
const char *error = "Unable to allocate memory in background process";

void background_killer(int index) { pthread_cancel(proc_threads[index]); }

void *background_handler(int index, double start) {
  starts[index] = start;
  successes[index] = 0;
  pthread_create(&proc_threads[index], NULL, bg_funcs[index], raw_datas[index]);
  void *new;
  pthread_join(proc_threads[index], &new);
  if (new != PTHREAD_CANCELED) {
    raw_datas[index] = new;
  }
  successes[index] = 1;
  return NULL;
}

void *background_loop() {
  for (;;) {
    for (int i = 0; i < bg_len; i++) {
      double now = clock();
      double time = (double)(now - last_ran) / CLOCKS_PER_SEC;
      if (time >= delays[i]) {
        if (starts[i] + kills[i] < now && successes[i] == 0) {
          background_killer(i);
        } else if (successes[i] == 1) {
          background_handler(i, now);
        }
      }
    }
  }
  pthread_exit(NULL);
}

void background_init() {
  signal(SIGTERM, background_stop_and_shutdown);
  signal(SIGCHLD, SIG_IGN);
  proc_threads = malloc(bg_cap * sizeof(pthread_t));
  check_pointer_throw(proc_threads, error);
  bg_funcs = malloc(bg_cap * sizeof(BG_FUNC));
  check_pointer_throw(bg_funcs, error);
  raw_datas = malloc(bg_cap * sizeof(void *));
  check_pointer_throw(raw_datas, error);
  successes = malloc(bg_cap * sizeof(int));
  check_pointer_throw(successes, error);
  starts = malloc(bg_cap * sizeof(clock_t));
  check_pointer_throw(starts, error);
  kills = malloc(bg_cap * sizeof(int));
  check_pointer_throw(kills, error);
  delays = malloc(bg_cap * sizeof(int));
  check_pointer_throw(delays, error);
}

int background_add(int milliseconds, void *raw_data, void *(*func)(void *),
                   int kill_milliseconds) {
  if (bg_cap == bg_len) {
    bg_cap *= 2;
    proc_threads = realloc(proc_threads, bg_cap * sizeof(pthread_t));
    check_pointer_throw(proc_threads, error);
    bg_funcs = realloc(bg_funcs, bg_cap * sizeof(BG_FUNC));
    check_pointer_throw(bg_funcs, error);
    successes = realloc(successes, bg_cap * sizeof(int));
    check_pointer_throw(successes, error);
    starts = realloc(starts, bg_cap * sizeof(clock_t));
    check_pointer_throw(starts, error);
    raw_datas = realloc(raw_datas, bg_cap * sizeof(void *));
    check_pointer_throw(raw_datas, error);
    kills = realloc(kills, bg_cap * sizeof(int));
    check_pointer_throw(kills, error);
    delays = realloc(delays, bg_cap * sizeof(int));
    check_pointer_throw(delays, error);
  }
  delays[bg_len] = milliseconds;
  raw_datas[bg_len] = raw_data;
  bg_funcs[bg_len] = func;
  successes[bg_len] = 1;
  starts[bg_len] = (clock_t)NULL;
  kills[bg_len] = kill_milliseconds;
  bg_len++;
  return bg_len - 1;
}

void *background_read(int index) { return raw_datas[index]; }
void background_start() {
  last_ran = clock();
  pthread_create(&bg_thread, NULL, &background_loop, NULL);
}

void background_stop_and_shutdown() {
  for (int i = 0; i < bg_len; i++) {
    pthread_cancel(proc_threads[i]);
  }
  free(proc_threads);
  free(bg_funcs);
  free(raw_datas);
  free(successes);
  free(starts);
  free(kills);
}
