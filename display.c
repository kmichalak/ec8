#include "display.h"

#include <SDL2/SDL.h>


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

const char *byte_to_binary2(uint64_t x)
{
    static char b[65];
    b[0] = '\0';

    int z;
    for (z = 512; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void put_pixels(Display *display, unsigned char *sprite, 
 				 unsigned short sprite_size,
 				 unsigned char x, unsigned char y) {

 	uint64_t line = 0;
 	for (int i=0; i<sprite_size; i++) {
 		uint64_t sprite_line = sprite[i];
 		line = (sprite_line << (64 - 8 - x));

 		int64_t buff = display->screen[y + i] ^ line;
 		int64_t collision_buff = display->screen[y + i] | line;

 		display->collision_found = buff != collision_buff;

 		display->screen[y + i] = buff; 			
 	}

 	for (int yy=0; yy<32; yy++) {
		uint64_t screen_line = display->screen[yy];
		for (int xx=0; xx<64; xx++) {
			bool pixel_on = (screen_line >> xx) & 0x1;
			if (pixel_on) {
				put_pixel(64 - xx, yy, display);
			}
		}
	}	

	SDL_RenderPresent(display->renderer);
}

void clear(Display *display) {
	memset(display->screen, 0, sizeof(display->screen));
	SDL_RenderClear(display->renderer);
	SDL_RenderPresent(display->renderer);
}

void init_display(Display *display) {
	memset(display->screen, 0, sizeof(uint64_t) * 32);

	display->put_pixels = put_pixels;
	display->write_collision_state = write_collision_state;
	display->clear_screen = clear;

	int status = 0;
	status = SDL_Init(SDL_INIT_VIDEO);
	
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

		SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
		SDL_RenderClear(display->renderer);
		SDL_RenderPresent(display->renderer);
		
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

