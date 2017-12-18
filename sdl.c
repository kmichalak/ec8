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

void init_screen(Screen *screen, ScreenSize *size) {
	int status = 0;
	status = SDL_Init(SDL_INIT_VIDEO);
	if (status == 0) {
		screen->size.width = size->width;
		screen->size.height = size->height;
		screen->size.scale = size->scale;
		screen->window = SDL_CreateWindow(
			"Chil-8",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screen->size.width * screen->size.scale,
			screen->size.height * screen->size.scale,
			SDL_WINDOW_SHOWN
		);	

		screen->renderer = SDL_CreateRenderer(
			screen->window, -1, SDL_RENDERER_SOFTWARE
		);

		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 0);
		clear_screen(screen);
	} else {
		printf("Something went wrong. SDL initialisation status: %d\n", status);
		// raise error here
	}
}

static void draw_pixel(unsigned x, unsigned y, Screen *screen) {
	SDL_Rect pixel_rect;
	pixel_rect.x = x * screen->size.scale;
	pixel_rect.y = y * screen->size.scale;
	pixel_rect.w = screen->size.scale;
	pixel_rect.h = screen->size.scale;

	Uint8 r, g, b, a;

	SDL_GetRenderDrawColor(screen->renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(screen->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(screen->renderer, &pixel_rect);
	SDL_SetRenderDrawColor(screen->renderer, r, g, b, a);
}

void draw_pixels(Screen *screen, uint64_t *pixels) {
	SDL_RenderClear(screen->renderer);

 	for (int yy=0; yy<screen->size.height; yy++) {
		uint64_t screen_line = pixels[yy];
		for (int xx=0; xx<screen->size.width; xx++) {
			bool pixel_on = (screen_line >> xx) & 0x1;
			if (pixel_on) {
				draw_pixel(screen->size.width - xx, yy, screen);
			}
		}
	}	

	SDL_RenderPresent(screen->renderer);	
}

void destroy_screen(Screen *screen) {
	SDL_DestroyWindow(screen->window);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
	SDL_DestroyWindow(screen->window);
	SDL_Quit();
}