#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>
#include <stdbool.h>

#include "sdl.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 20
#define SPRITE_SIZE 8

typedef struct Display Display;

struct Display {
    uint64_t screen[32];    // 64x32 pixels
    bool collision_found;
    Screen *sdl_screen;
    void (*put_pixels)(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y);
    void (*write_collision_state)(Display *display, unsigned char *cpu_reg);
    void (*clear_screen)();
};

void init_display(Display *display);

void destroy_display(Display *display);

#endif