#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"
// #include "timer.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>

char wait_for_key() {
	bool waits_for_key = true;

	SDL_Event event;
	char key_index;
	printf("Waiting for key \n");

	while (waits_for_key) {
		while (SDL_PollEvent(&event)) {
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type == SDL_KEYDOWN) {
				SDL_Scancode keyIndex = event.key.keysym.scancode;

				switch (keyIndex) {
					case SDL_SCANCODE_0:
					case SDL_SCANCODE_1:
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_5:
					case SDL_SCANCODE_6:
					case SDL_SCANCODE_7:
					case SDL_SCANCODE_8:
					case SDL_SCANCODE_9:
						printf("Got the key\n");
						key_index = SDL_SCANCODE_0 - keyIndex;
						waits_for_key = false;	
						break;
					default: 
						break;
				}
			
			}	
		}
	}

	return key_index;
}

static void run_cpu_cycle(Timer *timer, void *data) {
	// printf("Handling tick\n");
	Cpu *cpu = (Cpu *) data;

	if (cpu->is_running(cpu)) {
		
		SDL_Event event;
		char key_index;

		while (SDL_PollEvent(&event)) {
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type == SDL_KEYDOWN) {
				SDL_Scancode keyIndex = event.key.keysym.scancode;

				switch (keyIndex) {
					case SDL_SCANCODE_0:
						cpu->key[0] = 0x1;
						break;
					case SDL_SCANCODE_1:
						cpu->key[1] = 0x1;
						break;
					case SDL_SCANCODE_2:
						cpu->key[2] = 0x1;
						break;
					case SDL_SCANCODE_3:
						cpu->key[3] = 0x1;
						break;
					case SDL_SCANCODE_4:
						cpu->key[4] = 0x1;
						break;
					case SDL_SCANCODE_5:
						cpu->key[5] = 0x1;
						break;
					case SDL_SCANCODE_6:
						cpu->key[6] = 0x1;
						break;
					case SDL_SCANCODE_7:
						cpu->key[7] = 0x1;
						break;
					case SDL_SCANCODE_8:
						cpu->key[8] = 0x1;
						break;
					case SDL_SCANCODE_9:
						cpu->key[9] = 0x1;
						break;
						printf("Got the key\n");
						key_index = SDL_SCANCODE_0 - keyIndex;
						// waits_for_key = false;	
						break;
					case SDL_SCANCODE_Q:
						cpu->running = false;
						break;
					default: 
						break;
				}
			}	
			else if (event.type == SDL_KEYUP) {
				SDL_Scancode keyIndex = event.key.keysym.scancode;

				switch (keyIndex) {
					case SDL_SCANCODE_0:
						cpu->key[0] = 0x0;
						break;
					case SDL_SCANCODE_1:
						cpu->key[1] = 0x0;
						break;
					case SDL_SCANCODE_2:
						cpu->key[2] = 0x0;
						break;
					case SDL_SCANCODE_3:
						cpu->key[3] = 0x0;
						break;
					case SDL_SCANCODE_4:
						cpu->key[4] = 0x0;
						break;
					case SDL_SCANCODE_5:
						cpu->key[5] = 0x0;
						break;
					case SDL_SCANCODE_6:
						cpu->key[6] = 0x0;
						break;
					case SDL_SCANCODE_7:
						cpu->key[7] = 0x0;
						break;
					case SDL_SCANCODE_8:
						cpu->key[8] = 0x0;
						break;
					case SDL_SCANCODE_9:
						cpu->key[9] = 0x0;
						break;
						printf("Got the key\n");
						key_index = SDL_SCANCODE_0 - keyIndex;
						// waits_for_key = false;	
						break;
					case SDL_SCANCODE_Q:
						cpu->running = false;
						break;
					default: 
						break;
				}
			}
		}

		// printf("CPU is running\n");
		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);
		if (cpu->dt > 0) {		
			printf("DT: %d\n", cpu->dt);
			cpu->dt = cpu->dt - 1;
		}
		if (cpu->st > 0) {
			printf("ST: %d\n", cpu->st);
			cpu->st = cpu->st - 1;
		}



		// cpu->deelay_timer->tick(cpu->deelay_timer, cpu);
		// cpu->sound_timer->tick(cpu->sound_timer, cpu);

		// printf("PC %x\n", cpu->PC);
		// printf("I %x\n", cpu->I);
		// for (int  i=0; i<16; i++) {
		// 	printf("V%d %x\n", i, cpu->registers[i]);
		// }

		// getchar();

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