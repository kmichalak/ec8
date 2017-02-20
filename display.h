#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct Display Display;

struct Display {
    uint64_t screen[32];    // 64x32 pixels
    bool collision_found;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void (*put_pixels)(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y);
    void (*write_collision_state)(Display *display, unsigned char *cpu_reg);
};

void init_display(Display *display);

void destroy_display(Display *display);

#endif