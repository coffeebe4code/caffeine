#pragma once

void background_init();
int background_add(int milliseconds, void *raw_data, void *(*func)(void *));
void *background_read(int index);
void background_start();
void background_stop_and_shutdown();
