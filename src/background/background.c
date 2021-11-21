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
int bg_len = 0;
int bg_cap = 25;
static clock_t last_ran;

void *background_handler(int index, double start) {

  starts[index] = start;
  successes[index] = 0;
  raw_datas[index] = raw_datas[index] = bg_funcs[index](raw_datas[index]);
  successes[index] = 1;
  return NULL;
}

void *background_loop(void *arg) {
  for (;;) {
    for (int i = 0; i < bg_len; i++) {
      double now = clock();
      double time = (double)(now - last_ran) / CLOCKS_PER_SEC;
      if (time >= (delays[i] + now)) {

        // TODO:: create a background handler
      }
    }
  }
  pthread_exit(NULL);
}

void background_init() {
  signal(SIGTERM, background_stop_and_shutdown);
  signal(SIGCHLD, SIG_IGN);
  proc_threads = malloc(bg_cap * sizeof(pthread_t));
  bg_funcs = malloc(bg_cap * sizeof(BG_FUNC));
  raw_datas = malloc(bg_cap * sizeof(void *));
  successes = malloc(bg_cap * sizeof(int));
  starts = malloc(bg_cap * sizeof(clock_t));
}

int background_add(int milliseconds, void *raw_data, void *(*func)(void *)) {
  if (bg_cap == bg_len) {
    bg_cap *= 2;
    proc_threads = realloc(proc_threads, bg_cap * sizeof(pthread_t));
    bg_funcs = realloc(bg_funcs, bg_cap * sizeof(BG_FUNC));
    successes = realloc(successes, bg_cap * sizeof(int));
    starts = realloc(starts, bg_cap * sizeof(clock_t));
    raw_datas = realloc(raw_datas, bg_cap * sizeof(void *));
  }
  delays[bg_len] = milliseconds;
  raw_datas[bg_len] = raw_data;
  bg_funcs[bg_len] = func;
  successes[bg_len] = 1;
  starts[bg_len] = (clock_t)NULL;
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
}
