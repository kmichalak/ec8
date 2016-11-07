#include <stdlib.h>
#include "ck8.h"

void initialize() {
	// initialize registers
	// setup memory 
}

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

opcode_handler handlers[16] = {
	handle_0, 
	jump,
	call_subrutine,
	vx_equals,
	vx_not_equals,
	vx_equals_vy,
	set_vx, 
	add_to_vx, 
	handle_8, 
	vx_not_equals_vy,
	set_prog_counter,
	offset_jump,
	random_vx,
	draw,
	handle_key,
	handle_f
};



void emulate_cycle(Cpu *cpu) {
	// fetch opcode
	cpu->opcode = cpu->memory[cpu->PC] << 8 | cpu->memory[cpu->PC + 1];

	// decode 
	(*handlers[cpu->opcode & 0xF000])(cpu->opcode);

	// update timers
}



int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));

	initialize();

	while(1) {
		emulate_cycle(cpu);
		// draw graphics if draw flag is set
		// set keys
	}

	return 0;
}