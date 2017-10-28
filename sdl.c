#include "sdl.h"


#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

static uint8_t supported_scancodes[16] = 
{
	SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, 
	SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, 
	SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
	SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F
};

void get_keyboard_state(unsigned char* kbd, bool* cpu_running) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			SDL_Scancode key_scancode = event.key.keysym.scancode;
			if (key_scancode == SDL_SCANCODE_Q) {
				*cpu_running = false;
			}
			for (int key_idx=0; key_idx < KBD_SIZE; key_idx++) {
				
				if (supported_scancodes[key_idx] == key_scancode) {
					kbd[key_idx] = 1;
				}

			}	
		} else if (event.type == SDL_KEYUP) {
			SDL_Scancode key_scancode = event.key.keysym.scancode;
			for (int key_idx=0; key_idx < KBD_SIZE; key_idx++) {

				if (supported_scancodes[key_idx] == key_scancode) {
					kbd[key_idx] = 0;
				}

			}
		}
	}
}

void clear_screen(Screen *screen) {
	SDL_RenderClear(screen->renderer);
	SDL_RenderPresent(screen->renderer);
}

void init_screen(Screen *screen) {
	screen->window = SDL_CreateWindow(
		"Chil-8",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen->width * screen->scale,
		screen->height * screen->scale,
		SDL_WINDOW_SHOWN
	);	

	screen->renderer = SDL_CreateRenderer(
		screen->window, -1, SDL_RENDERER_SOFTWARE
	);

	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 0);
	clear_screen(screen);
}

