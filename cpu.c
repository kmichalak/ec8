#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "opcodes.h"
#include "display.h"

opcode_handler ops_handlers[16];

static void fetch_opcode(Cpu *cpu) {
	cpu->opcode = cpu->memory[cpu->PC] << 8 | cpu->memory[cpu->PC + 1];
}

static void handle_opcode(Cpu *cpu) {
	// single opcode is two bytes (16 bits) so if we shift it by 12 bits
	// we will receive opcode group marker
	unsigned short opcode_group = (cpu->opcode & 0xF000) >> 12;

	opcode_handler handler = ops_handlers[opcode_group];

	handler(cpu);
}

static void put_pixels(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y) {
	uint64_t line;

	for (int i=0; i<sprite_size; i++) {
		line = (line << 8) | sprite[i];
	}

	line = line << (64 - sprite_size * 8 - x);
	display->screen[y] = display->screen[y] ^ line;
}

void initialize(Cpu *cpu) {
	cpu->PC = 0;
	cpu->I = 0;
	cpu->sp = 0;

	cpu->display = malloc(sizeof(Display));
	memset(cpu->display->screen, 0, sizeof(cpu->display->screen));
	memset(cpu->memory, 0, sizeof(cpu->memory));
	memset(cpu->registers, 0, sizeof(cpu->registers));

	cpu->fetch_opcode = fetch_opcode;
	cpu->handle_opcode = handle_opcode;
	cpu->display->put_pixels = put_pixels;
	
	ops_handlers[0x0] = handle_0;
	ops_handlers[0x1] = jump;
	ops_handlers[0x2] = call_subrutine;
	ops_handlers[0x3] = vx_equals;
	ops_handlers[0x4] = vx_not_equals;
	ops_handlers[0x5] = vx_equals_vy;
	ops_handlers[0x6] = set_vx;
	ops_handlers[0x7] = add_to_vx;
	ops_handlers[0x8] = handle_8;
	ops_handlers[0x9] = vx_not_equals_vy;
	ops_handlers[0xA] = set_prog_counter;
	ops_handlers[0xB] = offset_jump;
	ops_handlers[0xC] = random_vx;
	ops_handlers[0xD] = draw;
	ops_handlers[0xE] = handle_key;
	ops_handlers[0xF] = handle_f;
}

void shutdown(Cpu *cpu) {
	free(cpu->display);
}