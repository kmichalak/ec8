#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"
#include "sdl.h"

static void run_cpu_cycle(Timer *timer, void *data) {
	Cpu *cpu = (Cpu *) data;

	if (cpu->is_running(cpu)) {
		
		get_keyboard_state(cpu->key, &(cpu->running));

		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);
		if (cpu->dt > 0) {		
			cpu->dt = cpu->dt - 1;
		}
		if (cpu->st > 0) {
			cpu->st = cpu->st - 1;
		}

		timer->counter = 60;
	} else {
		printf("Shutting down CPU\n");
		timer->counter = 0;
		timer->enabled = false;
	}
}

int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));
	initialize(cpu);

	Timer *main_timer = malloc(sizeof(Timer));
	init_timer(main_timer, cpu, run_cpu_cycle);
	shutdown_cpu(cpu);

	return 0;
};