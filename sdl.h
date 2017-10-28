#ifndef SDL_H
#define SDL_H

#include <stdbool.h>

#define KBD_SIZE 16

void get_keyboard_state(unsigned char* kbd, bool* cpu_running);

#endif