#include "ck8.h"

void initialize() {
	// initialize registers
	// setup memory 
}

// 00EX
void handle_0(unsigned short opcode) {}

// 00E0
void clear_screen(unsigned short optcode) {}

// 00EE
void return_from_rsubutine(unsigned short optcode) {}

// 1NNN
void jump(unsigned short optcoder) {}

// 2NNN
void call_subrutine(unsigned short optcode) {}

// 3XNN
void vx_equals(unsigned short optcode) {}

// 4XNN
void vx_not_equals(unsigned short optcode) {}

// 5XY0
void vx_equals_vy(unsigned short optcode) {}

// 6XNN
void set_vx(unsigned short optcode) {}

// 7XNN
void add_to_vx(unsigned short optcode) {}

// 8XYN
void handle_8(unsigned short optcode) {}

// 9XY0
void vx_not_equals_vy(unsigned short optcode) {}

// ANNN
void set_prog_counter(unsigned short optcode) {}

// BNNN
void offset_jump(unsigned short optcode) {}

// CXNN
void random_vx(unsigned short optcode) {}

// DXYN
void draw(unsigned short optcode) {}

// EXNN
void handle_key(unsigned short optcode) {}

// FXNN
void handle_f(unsigned short optcode) {}


void (*handlers[16])(unsigned short opcode) = {
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


void emulate_cycle() {
	// fetch opcode
	opcode = memory[PC] << 8 | memory[PC + 1];

	// decode 
	(*handlers[opcode & 0xF000])(opcode);

	// update timers
}



int main(int argc, char* argv[]) {

	initialize();

	while(1) {
		emulate_cycle();
		// draw graphics if draw flag is set
		// set keys
	}

	return 0;
}