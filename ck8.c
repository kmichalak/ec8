#include <stdlib.h>
#include "ck8.h"


// 00EX
void handle_0(unsigned short opcode) {}

// 00E0
void clear_screen(unsigned short opcode) {}

// 00EE
void return_from_rsubutine(unsigned short opcode) {}

// 1NNN
void jump(unsigned short opcoder) {}

// 2NNN
void call_subrutine(unsigned short opcode) {}

// 3XNN
void vx_equals(unsigned short opcode) {}

// 4XNN
void vx_not_equals(unsigned short opcode) {}

// 5XY0
void vx_equals_vy(unsigned short opcode) {}

// 6XNN
void set_vx(unsigned short opcode) {}

// 7XNN
void add_to_vx(unsigned short opcode) {}

// 8XYN
void handle_8(unsigned short opcode) {}

// 9XY0
void vx_not_equals_vy(unsigned short opcode) {}

// ANNN
void set_prog_counter(unsigned short opcode) {}

// BNNN
void offset_jump(unsigned short opcode) {}

// CXNN
void random_vx(unsigned short opcode) {}

// DXYN
void draw(unsigned short opcode) {}

// EXNN
void handle_key(unsigned short opcode) {}

// FXNN
void handle_f(unsigned short opcode) {}


int emulate_cycle(Cpu *cpu) {
	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);
	// update timers
	return 0;
}

void fetch_opcode(Cpu *cpu) {
	cpu->opcode = cpu->memory[cpu->PC] << 8 | cpu->memory[cpu->PC + 1];
}

void handle_opcode(Cpu *cpu) {;
	cpu->handlers[cpu->opcode & 0xF000](cpu->opcode);
}

void initialize(Cpu *cpu) {
	cpu->run_cycle = emulate_cycle;
	cpu->fetch_opcode = fetch_opcode;
	cpu->handle_opcode = handle_opcode;
	cpu->handlers[0] = handle_0;
	cpu->handlers[1] = jump;
	cpu->handlers[2] = call_subrutine;
	cpu->handlers[3] = vx_equals;
	cpu->handlers[4] = vx_not_equals;
	cpu->handlers[5] = vx_equals_vy;
	cpu->handlers[6] = set_vx;
	cpu->handlers[7] = add_to_vx;
	cpu->handlers[8] = handle_8;
	cpu->handlers[9] = vx_not_equals_vy;
	cpu->handlers[0xA] = set_prog_counter;
	cpu->handlers[0xB] = offset_jump;
	cpu->handlers[0xC] = random_vx;
	cpu->handlers[0xD] = draw;
	cpu->handlers[0xE] = handle_key;
	cpu->handlers[0xF] = handle_f;
}

int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));

	initialize(cpu);

	while(1) {
		// run CPU cycle
		cpu->run_cycle(cpu);
		// draw graphics if draw flag is set
		// set keys
	}

	return 0;
};