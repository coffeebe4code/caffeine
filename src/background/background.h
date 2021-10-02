#pragma once



void background_init();
void background_add_spin(int milliseconds, void * raw_data, void (*func)(void *)); 
void background_add_mutex(int milliseconds, void * raw_data, void (*func)(void *)); 
void background_add_futex(int milliseconds, void * raw_data, void (*func)(void *)); 
void background_add_delayed(int milliseconds, void * raw_data, int delay_free, void (*func)(void *)); 
void background_start();
void background_stop();
