#include "display.h"
#include "sdl.h"

#include <SDL2/SDL.h>


void write_collision_state(Display *display, unsigned char *cpu_reg) {
	*cpu_reg = display->collision_found;
}

void put_pixels(Display *display, unsigned char *sprite, 
 				 unsigned short sprite_size,
 				 unsigned char x, unsigned char y) {


 	uint64_t line = 0;
 	for (int i=0; i<sprite_size; i++) {
 		uint64_t sprite_line = sprite[i];
 		int64_t shift_size = DISPLAY_WIDTH - SPRITE_SIZE - x + 1;
 		if (shift_size > 0) {
 			line = (sprite_line << shift_size);
 		} else {
 			line = (sprite_line >> -shift_size);
 		}

 		int64_t buff = display->screen[y + i] ^ line;
 		int64_t collision_buff = display->screen[y + i] | line;

 		display->collision_found = buff != collision_buff;

 		display->screen[y + i] = buff; 			
 	}

 	draw_pixels(display->sdl_screen, display->screen);
}

void clear(Display *display) {
	memset(display->screen, 0, sizeof(display->screen));
	clear_screen(display->sdl_screen);
}

void init_display(Display *display) {
	memset(display->screen, 0, sizeof(uint64_t) * DISPLAY_HEIGHT);

	display->put_pixels = put_pixels;
	display->write_collision_state = write_collision_state;
	display->clear_screen = clear;

	ScreenSize screen_size;
	screen_size.width = DISPLAY_WIDTH;
	screen_size.height = DISPLAY_HEIGHT;
	screen_size.scale = DISPLAY_SCALE;
	display->sdl_screen = malloc(sizeof(Screen));
	init_screen(display->sdl_screen, &screen_size);
}

void destroy_display(Display *display) {
	destroy_screen(display->sdl_screen);
} 

