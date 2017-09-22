#include "display.h"

#include <SDL2/SDL.h>

 void put_pixels(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y) {
	uint64_t line;

	for (int i=0; i<sprite_size; i++) {
		line = (line << 8) | sprite[i];
	}

	line = line << (64 - sprite_size * 8 - x);
	int64_t buff = display->screen[y] ^ line;

	int64_t collision_buff = display->screen[y] | line;

	display->collision_found = buff != collision_buff;

	display->screen[y] = display->screen[y] ^ line;
}

 void write_collision_state(Display *display, unsigned char *cpu_reg) {
	*cpu_reg = display->collision_found;
}

 void put_pixel(unsigned x, unsigned y, Display *display) {
	SDL_Rect pixel_rect;
	pixel_rect.x = x * 10;
	pixel_rect.y = y * 10;
	pixel_rect.w = 10;
	pixel_rect.h = 10;

	Uint8 r;
	Uint8 g; 
	Uint8 b;
	Uint8 a;

	SDL_GetRenderDrawColor(display->renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(display->renderer, &pixel_rect);
	SDL_SetRenderDrawColor(display->renderer, r, g, b, a);
}


void init_display(Display *display) {
	// memset(display, 0, sizeof(display));
	memset(display->screen, 0, sizeof(display->screen));

	display->put_pixels = put_pixels;
	display->write_collision_state = write_collision_state;

	int status = 0;
	status = SDL_Init(SDL_INIT_VIDEO);
	//----------------------------------------
	if (status == 0) {

		display->window = SDL_CreateWindow(
			"Chip-8", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 
			640, 320,
			SDL_WINDOW_SHOWN
		);

		display->renderer = SDL_CreateRenderer(
			display->window, -1,  SDL_RENDERER_SOFTWARE
		);


		for (int k=0; k < 32; k++) {

			SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
			
			SDL_RenderClear(display->renderer);

			
			for (int i=0; i<32; i++) {
				if (i % 2 == 0) {
					if (k % 2 == 0) 
					display->screen[i] = 0x5555555555555555;
					else 
					display->screen[i] = 0xaaaaaaaaaaaaaaaa;
				} else {
					if (k % 2 == 0)
					display->screen[i] = 0xaaaaaaaaaaaaaaaa;
					else
					display->screen[i] = 0x5555555555555555;
				}
			}

			for (int i=0; i<32; i++) {
				uint64_t screen_line = display->screen[i];
				for (int j=0; j<64; j++) {
					bool enabled = (screen_line >> j) & 0x1;
					if (enabled) {
						put_pixel(j, i, display);
					}
				}
			}

			// put_pixel(2, 2, display);
			// put_pixel(2, 3, display);
			// put_pixel(2, 4, display);

			// SDL_FilRect()
			// SDL_RenderDrawRect(display->renderer, &pixel_rect);


			// SDL_RenderClear(display->renderer);
			SDL_RenderPresent(display->renderer);
			SDL_Delay(10);
		}

	}

	//----------------------------------------
}

void destroy_display(Display *display) {
	// SDL_Delay(3000);
	// SDL_DestroyWindow(display->window);
	// SDL_Renderer *renderer = SDL_GetRenderer(display->window);
	// SDL_DestroyRenderer(display->renderer);
	// SDL_DestroyWindow(display->window);
	// SDL_DestroyWindow(display->window);
	// SDL_Quit();
} 

