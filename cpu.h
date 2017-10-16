#ifndef CPU_H
#define CPU_H

#include "display.h"
#include "timer.h"

typedef struct Cpu Cpu;

typedef void (* opcode_handler)(Cpu *cpu);

struct Cpu {
	unsigned char memory[4096];
	unsigned char registers[16];
	unsigned short opcode;
	unsigned short I;
	unsigned short PC;
	unsigned short stack[16];
	unsigned short sp;
	unsigned char key[16];
	unsigned char dt;
	unsigned char st;
	Timer *deelay_timer;
	Timer *sound_timer;
	void (*fetch_opcode)(Cpu *cpu);
	void (*handle_opcode)(Cpu *cpu);
	bool (*is_running)(Cpu *cpu);

	Display *display;
	bool running;
};

void initialize(Cpu *cpu);

void shutdown(Cpu *cpu);

#endif