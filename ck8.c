#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"

// opcode_handler ops_handlers[16];

// int emulate_cycle(Cpu *cpu) {
// 	cpu->fetch_opcode(cpu);
// 	cpu->handle_opcode(cpu);
// 	// update timers
// 	return 0;
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