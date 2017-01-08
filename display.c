#include "display.h"

static void put_pixels(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y) {
	uint64_t line;

	for (int i=0; i<sprite_size; i++) {
		line = (line << 8) | sprite[i];
	}

	line = line << (64 - sprite_size * 8 - x);
	int64_t buff = display->screen[y] ^ line;

	int64_t collision_buff = display->screen[y] | line;

	bool no_collision = buff == collision_buff;
	display->collision_found = !no_collision;

	display->screen[y] = display->screen[y] ^ line;
}

static void write_collision_state(Display *display, unsigned char *cpu_reg) {
	*cpu_reg = display->collision_found;
}

void init_display(Display *display) {
	display->put_pixels = put_pixels;
	display->write_collision_state = write_collision_state;
}
