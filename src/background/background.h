#pragma once

void background_init();
void background_add(int milliseconds, void *raw_data, void (*func)(void *));
void background_start();
void background_stop_and_shutdown();
