#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdbool.h>

#include "cpu.h"
#include "display.h"
#include "opcodes.h"

opcode_handler ops_handlers[16];

static unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static void fetch_opcode(Cpu *cpu) {
	cpu->opcode = cpu->memory[cpu->PC] << 8 | cpu->memory[cpu->PC + 1];
}

static void handle_opcode(Cpu *cpu) {
	// single opcode is two bytes (16 bits) so if we shift it by 12 bits
	// we will receive opcode group marker
	unsigned short opcode_group = (cpu->opcode & 0xF000) >> 12;
	printf("Handling opcode %x\n", cpu->opcode);
	opcode_handler handler = ops_handlers[opcode_group];

	handler(cpu);
}

static bool is_running(Cpu *cpu) {
	return cpu->running;
}

static void decrement_dt(Timer *timer, void *data) {
	Cpu *cpu = (Cpu *) data;
	printf("Decrementing DT: %d -> %d\n", cpu->dt, cpu->dt - 1);
	cpu->dt--;
	if (cpu->dt == 0) {
		timer->enabled = false;
	} else {
		timer->enabled = true;
	}
}

static void decrement_st(Timer *timer, void *data) {
	Cpu *cpu = (Cpu *) data;
	cpu->st--;
	if (cpu->st == 0) {
		timer->enabled = false;
	} else {
		timer->enabled = true;
	}
}

void initialize(Cpu *cpu) {
	cpu->PC = 0x200;
	cpu->I = 0x200;
	cpu->sp = 0;

	memset(cpu->memory, 0, sizeof(char) * 4096);
	memcpy(cpu->memory, chip8_fontset, sizeof(char) * 80);

	memset(cpu->registers, 0, sizeof(char) * 16);
	memset(cpu->stack, 0, sizeof(char) * 16);


	cpu->fetch_opcode = fetch_opcode;
	cpu->handle_opcode = handle_opcode;
	cpu->is_running = is_running;
	cpu->display = malloc(sizeof(Display));
	init_display(cpu->display);
	
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

	cpu->dt = 0;
	cpu->st = 0;
	// cpu->deelay_timer = malloc(sizeof(Timer));
	// cpu->sound_timer = malloc(sizeof(Timer));

	// init_timer2(cpu->deelay_timer, cpu, decrement_dt);
	// init_timer2(cpu->sound_timer, cpu, decrement_st);

	int mem_pos = 0x200;

	FILE *fl = fopen("/home/kmichalak/PONG", "r");  
    fseek(fl, 0, SEEK_END);  
    long len = ftell(fl);  
    char *ret = malloc(len);  
    fseek(fl, 0, SEEK_SET);  
    fread(ret, 1, len, fl);  
    fclose(fl);  

    for (int a = 0; a < len; a++) {
    	printf("Rewriting byte %x\n", ret[a]);
    	cpu->memory[mem_pos + a] = ret[a];
    }


	cpu->running = true;
}

void shutdown_cpu(Cpu *cpu) {
	printf("Shutdown method called\n");
	// if (cpu == 0) {
	// 	printf("CPU is already destroyed\n");
	// }
	// if (cpu->display == 0) {
	// 	printf("Display is already destroyed\n");
	// } 

	destroy_display(cpu->display);
	free(cpu->display);
}