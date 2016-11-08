#include <stdlib.h>
// #include "opcodes.h"
#include "cpu.h"
#include "ck8.h"

opcode_handler ops_handlers[16];

int emulate_cycle(Cpu *cpu) {
	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);
	// update timers
	return 0;
}

// void fetch_opcode(Cpu *cpu) {
// 	cpu->opcode = cpu->memory[cpu->PC] << 8 | cpu->memory[cpu->PC + 1];
// }

// void handle_opcode(Cpu *cpu) {
// 	// cpu->handlers[cpu->opcode & 0xF000](cpu);
// 	unsigned short opcode_group = cpu->opcode & 0xF000;
// 	ops_handlers[opcode_group](cpu);

// }

// void initialize(Cpu *cpu) {
// 	// cpu->run_cycle = emulate_cycle;
// 	cpu->fetch_opcode = fetch_opcode;
// 	cpu->handle_opcode = handle_opcode;
	
// 	ops_handlers[0] = handle_0;
// 	ops_handlers[1] = jump;
// 	ops_handlers[2] = call_subrutine;
// 	ops_handlers[3] = vx_equals;
// 	ops_handlers[4] = vx_not_equals;
// 	ops_handlers[5] = vx_equals_vy;
// 	ops_handlers[6] = set_vx;
// 	ops_handlers[7] = add_to_vx;
// 	ops_handlers[8] = handle_8;
// 	ops_handlers[9] = vx_not_equals_vy;
// 	ops_handlers[0xA] = set_prog_counter;
// 	ops_handlers[0xB] = offset_jump;
// 	ops_handlers[0xC] = random_vx;
// 	ops_handlers[0xD] = draw;
// 	ops_handlers[0xE] = handle_key;
// 	ops_handlers[0xF] = handle_f;
// }

int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));
	initialize(cpu);
	// initialize(cpu);

	while(1) {
		// run CPU cycle
		// draw graphics if draw flag is set
		// set keys
	}

	return 0;
};