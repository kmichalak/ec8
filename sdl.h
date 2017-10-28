#ifndef SDL_H
#define SDL_H

#include <stdbool.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_render.h>

#define KBD_SIZE 16
#define DEFAULT_SCRN_WIDTH 64
#define DEFAULT_SCRN_HEIGHT 32
#define DEFAULT_SCRN_SCALE 10

typedef struct {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
	Uint8 x;
	Uint8 y;
} Pixel;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	Uint8 width;
	Uint8 height;
	Uint8 scale;
} Screen;

void get_keyboard_state(unsigned char* kbd, bool* cpu_running);

void init_screen(Screen *screen);
void clear_screen(Screen *screen);

#endif