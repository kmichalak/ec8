#ifndef CPU_H
#define CPU_H

#include "display.h"

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
	// opcode_handler handlers[16];
	// int (*run_cycle)(Cpu *cpu);
	void (*fetch_opcode)(Cpu *cpu);
	void (*handle_opcode)(Cpu *cpu);

	Display *display;
};

void initialize(Cpu *cpu);

void shutdown(Cpu *cpu);

#endif